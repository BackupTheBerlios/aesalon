/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/storage/RTree.h
*/

#ifndef AesalonStorage_RTree_H
#define AesalonStorage_RTree_H

#include <stdarg.h>
#include <list>
#include "Config.h"

#include "util/MessageSystem.h"
#include "Mempool.h"
#include "Bound.h"
#include "RTreePrivate.h"

namespace Storage {

/** An arbitrary-dimensional R-tree.
	Notes:
	- KeyType must support a basic assignment specifier and also the element zero, plus all the usual arithmetic operators.
		Using an elementary type (int, double, etc.) is a very good idea.
	- DataType can be any type, but pointers are probably the best idea.
	- 2 <= MinimumFactor <= MaximumFactor/2.
	- MaximumFactor should be a reasonably small number. Many linear searches through arrays of this size take place.
	- Dimensions should be as small as possible; many linear operations take place on this number.
	- FloatKey should be a version of Key that supports floating-point arithmetic (or at least non-integer division).
		If Key is already a floating-point type, then this can be omitted.
	Unless otherwise noted, all algorithms are from
		A Guttman, R-Trees: A Dynamic Index Structure for Spatial Searching, 1984.
*/

template<typename KeyType, typename DataType, int Dimensions, int MinimumFactor = 2, int MaximumFactor = 16, typename FloatKeyType=KeyType>
class RTree {
public:
	typedef Bound<KeyType, Dimensions> BoundType;
	typedef RTreePrivate::SearchVisitor<DataType, BoundType> SearchVisitorType;
private:
	typedef RTreePrivate::Node<DataType, BoundType, MaximumFactor> NodeType;
	typedef RTreePrivate::InternalNode<DataType, BoundType, MaximumFactor> InternalNodeType;
	typedef RTreePrivate::LeafNode<DataType, BoundType, MaximumFactor> LeafNodeType;
	
	NodeType *m_root;
public:
	RTree() {
		m_root = NULL;
		/* Some basic sanity checks. */
		if(MinimumFactor*2 > MaximumFactor) Message(Fatal, "RTree: MinimumFactor must be at most half of MaximumFactor.");
		if(MaximumFactor < 2) Message(Fatal, "RTree: MaximumFactor must be at least 2!");
	}
	
	~RTree() {
		
	}
	
	void search(const BoundType &bound, SearchVisitorType &visitor)
		{ searchHelper(bound, m_root, visitor); }
	
	void insert(const BoundType &bound, const DataType &data) {
		Message(Debug, "");
		Message(Debug, "");
		Message(Debug, "************ Inserting " << data);
		if(m_root == NULL) {
			Message(Debug, "Creating new root node.");
			LeafNodeType *leafRoot;
			AesalonPoolAlloc(LeafNodeType, leafRoot, LeafNodeType());
			leafRoot->addBranch(bound, data);
			m_root = leafRoot;
			return;
		}
		
		Message(Debug, "Looking for leaf node to insert into . . .");
		NodeType *node = insertChooseLeaf(bound);
		Message(Debug, "Chose " << node);
		
		Message(Debug, "Attempting to add data to leaf.");
		bool succeeded = node->asLeafNode()->addBranch(bound, data);
		if(succeeded) {
			Message(Debug, "Succeeded! Branch count is now " << node->branchCount());
			return;
		}
		Message(Debug, "Failed. Splitting node . . .");
		LeafNodeType *newLeaf = splitNode(node->asLeafNode());
		Message(Debug, "New node: " << newLeaf);
		adjustTree(node, newLeaf);
	}
	
	void remove(const BoundType &bound, const DataType &data) {
		LeafNodeType *leaf = removeFromLeaf(bound, data, m_root);
		
		condenseTree(leaf);
		
		if(!m_root->isLeaf() && m_root->branchCount() < MinimumFactor) {
			NodeType *newRoot = m_root->asInternalNode()->branch(0);
			/* LEAK m_root . . . */
			m_root = newRoot;
		}
	}
	
	void updateBound(const BoundType &oldBound, const DataType &data, const BoundType &newBound) {
		remove(oldBound, data);
		insert(newBound, data);
	}
private:
	void searchHelper(const BoundType &bound, NodeType *node, SearchVisitorType &visitor) {
		if(node == NULL) return;
		
		for(int branch = 0; branch < node->branchCount(); branch ++) {
			if(!bound.overlaps(node->branchBound(branch))) continue;
			
			if(node->isLeaf()) {
				visitor.visit(node->branchBound(branch), node->asLeafNode()->branch(branch));
			}
			else {
				Message(Debug, "Transitioning from " << (void *)((unsigned long)node & 0xfff) << " to " << (void *)((unsigned long)node->asInternalNode()->branch(branch) & 0xfff));
				searchHelper(bound, node->asInternalNode()->branch(branch), visitor);
			}
		}
	}
	
	NodeType *insertChooseLeaf(const BoundType &bound) {
		NodeType *node = m_root;
		while(!node->isLeaf()) {
			KeyType minCost;
			int minElement;
			for(int i = 0; i < node->branchCount(); i ++) {
				KeyType coverCost = node->branchBound(i).toCover(bound);
				Message(Debug, "Cover cost for branch #" << i << ": " << coverCost);
				if(i == 0 || coverCost < minCost) minElement = i, minCost = coverCost;
			}
			
			Message(Debug, "Choosing branch #" << minElement << " of " << node->branchCount());
			
			node = node->asInternalNode()->branch(minElement);
		}
		return node;
	}
	
	void adjustTree(NodeType *node, NodeType *newNode) {
		while(node != m_root) {
			InternalNodeType *parent = node->parent()->asInternalNode();
			
			int branch = parent->branch(node);
			parent->setBranchBound(branch, node->overallBound());
			
			if(newNode != NULL) {
				bool succeeded = parent->addBranch(newNode->overallBound(), newNode);
				if(!succeeded) {
					newNode = splitNode(parent);
				}
			}
			
			node = parent;
		}
		
		if(newNode != NULL) {
			Message(Debug, "Growing tree height . . .");
			InternalNodeType *newRoot;
			AesalonPoolAlloc(InternalNodeType, newRoot, InternalNodeType());
			newRoot->setDepth(newNode->depth()+1);
			newRoot->addBranch(node->overallBound(), node);
			newRoot->addBranch(newNode->overallBound(), newNode);
			m_root = newRoot;
		}
	}
	
	void linearSplitSeeds(NodeType *node, int *seeds) {
		KeyType highestStartValue[Dimensions] = {0};
		int highestStart[Dimensions] = {-1};
		KeyType lowestEndValue[Dimensions] = {0};
		int lowestEnd[Dimensions] = {-1};
		
		KeyType lowest[Dimensions];
		KeyType highest[Dimensions];
		
		for(int d = 0; d < Dimensions; d ++) {
			for(int i = 0; i <= MaximumFactor; i ++) {
				const BoundType &bound = node->branchBound(i);
				
				if(i == 0 || bound.start(d) > highestStartValue[d]) {
					highestStart[d] = i;
					highestStartValue[d] = bound.start(d);
				}
				if(i == 0 || bound.end(d) < lowestEndValue[d]) {
					lowestEnd[d] = i;
					lowestEndValue[d] = bound.end(d);
				}
				
				if(i == 0 || bound.start(d) < lowest[d]) lowest[d] = bound.start(d);
				if(i == 0 || bound.end(d) < highest[d]) highest[d] = bound.end(d);
			}
		}
		
		FloatKeyType greatestSeparation;
		int greatestSeparationDimension = -1;
		for(int d = 0; d < Dimensions; d ++) {
			KeyType size = highest[d] - lowest[d];
			if(size == 0) continue;
			
			if(highestStart[d] == lowestEnd[d]) continue;
			
			KeyType sep = highestStartValue[d] - lowestEndValue[d];
			FloatKeyType sepNormalized = FloatKeyType(sep) / size;
			
			if(greatestSeparationDimension == -1 || sepNormalized > greatestSeparation) {
				greatestSeparation = sepNormalized;
				greatestSeparationDimension = d;
			}
		}
		
		/* Check for degeneracy case . . . */
		if(greatestSeparationDimension == -1) {
			seeds[0] = 0;
			seeds[1] = 1;
		}
		else {
			seeds[0] = highestStart[greatestSeparationDimension];
			seeds[1] = lowestEnd[greatestSeparationDimension];
		}
	}
	
	template<typename SplitNodeType>
	SplitNodeType *splitNode(SplitNodeType *toSplit) {
		int seeds[2];
		linearSplitSeeds(toSplit, seeds);
		
		BoundType tsBound;
		BoundType nBound;
		
		int tsSeed = 0;
		
		SplitNodeType *newNode = NULL;
		AesalonPoolAlloc(SplitNodeType, newNode, SplitNodeType());
		
		newNode->setParent(toSplit->parent());
		if(seeds[0] > seeds[1]) {
			newNode->addBranch(toSplit->branchBound(seeds[0]), toSplit->branch(seeds[0]));
			nBound = toSplit->branchBound(seeds[0]);
			toSplit->removeBranch(seeds[0]);
			
			tsBound = toSplit->branchBound(seeds[1]);
			tsSeed = seeds[1];
		}
		else if(seeds[1] > seeds[0]) {
			newNode->addBranch(toSplit->branchBound(seeds[1]), toSplit->branch(seeds[1]));
			nBound = toSplit->branchBound(seeds[1]);
			toSplit->removeBranch(seeds[1]);
			
			tsBound = toSplit->branchBound(seeds[0]);
			tsSeed = seeds[0];
		}
		else {
			abort();
			Message(Fatal, "Both seeds for node-splitting are identical. Fix linearSplitSeeds.");
		}
		
		/* Iterate through branches. */
		/* NOTE: use reverse order for a slight speed increase (see implementation of removeNode). */
		for(int i = toSplit->branchCount()-1; i >= 0; i --) {
			/* Skip the seed. */
			if(i == tsSeed) continue;
			
			int adjI = i;
			if(i > tsSeed) adjI --;
			
			const BoundType &bound = toSplit->branchBound(i);
			
			/* Handle the minumum branching factor cases . . . */
			if(toSplit->branchCount() == MinimumFactor) {
				break;
			}
			else if(newNode->branchCount() + adjI == MinimumFactor) {
				for(; i >= 0; i --) {
					if(i == tsSeed) continue;
					
					newNode->addBranch(bound, toSplit->branch(i));
					toSplit->removeBranch(i);
				}
				break;
			}
			
			if(tsBound.toCover(bound) > nBound.toCover(bound)) {
				newNode->addBranch(bound, toSplit->branch(i));
				toSplit->removeBranch(i);
			}
			/* The other case is a fall-through, e.g. the branch is already in the correct node. */
		}
		
		newNode->setDepth(toSplit->depth());
		
		return newNode;
	}
	
	LeafNodeType *removeFromLeaf(const BoundType &bound, const DataType &data, NodeType *node) {
		if(node == NULL) return NULL;
		else if(node->isLeaf()) {
			for(int i = 0; i < node->branchCount(); i ++) {
				if(node->branchBound(i).overlaps(bound) && node->asLeafNode()->branch(i) == data) {
					node->asLeafNode()->removeBranch(i);
					return node->asLeafNode();
				}
			}
		}
		else {
			for(int i = 0; i < node->branchCount(); i ++) {
				if(node->branchBound(i).overlaps(bound)) {
					LeafNodeType *n = removeFromLeaf(bound, data, node->asInternalNode()->branch(i));
					if(n != NULL) return n;
				}
			}
		}
		return NULL;
	}
	
	void condenseTreeInsert(NodeType *toInsert) {
		const BoundType &bound = toInsert->overallBound();
		NodeType *node = m_root;
		while(node->depth() > (toInsert->depth()+1)) {
			KeyType minCost;
			int minElement;
			for(int i = 0; i < node->branchCount(); i ++) {
				KeyType coverCost = node->branchBound(i).toCover(bound);
				if(i == 0 || coverCost < minCost) minElement = i, minCost = coverCost;
			}
			
			node = node->asInternalNode()->branch(minElement);
		}
		
		if(!node->asInternalNode()->addBranch(bound, toInsert)) {
			InternalNodeType *newNode = splitNode(node->asInternalNode());
			adjustTree(node, newNode);
		}
	}
	
	void condenseTree(NodeType *node) {
		typename std::list<NodeType *> removedBranches;
		while(node != m_root) {
			/* The parent will always be an internal node, even if node itself is a leaf. */
			InternalNodeType *parent = node->parent()->asInternalNode();
			
			int branch = parent->branch(node);
			if(node->branchCount() < MinimumFactor) {
				parent->removeBranch(branch);
				removedBranches.push_back(node);
			}
			else {
				parent->setBranchBound(branch, node->overallBound());
			}
			
			node = parent;
		}
		
		/* Re-insert the removed branches into the tree. */
		for(typename std::list<NodeType *>::reverse_iterator i = removedBranches.rbegin(); i != removedBranches.rend(); --i) {
			condenseTreeInsert(*i);
		}
	}
};

} // namespace Storage

#endif
