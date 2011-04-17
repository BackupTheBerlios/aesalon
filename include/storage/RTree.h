/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/storage/RTree.h
*/

#ifndef AesalonStorage_RTree_H
#define AesalonStorage_RTree_H

#include <stdarg.h>
#include "Config.h"

#include "util/MessageSystem.h"
#include "Mempool.h"
#include "Bound.h"
#include "RTreePrivate.h"

namespace Storage {

/** An arbitrary-dimensional R-tree.
	Notes:
	- Key must support a basic assignment specifier and also the element zero, plus all the usual arithmetic operators.
		Using an elementary type is a very good idea.
	- Value must be either a basic type or a pointer.
	- Minimum <= Maximum/2. (Logic in code assumes this.)
	- Maximum <= some reasonable number. Many linear searches through lists of this size take place.
		A reasonable size is 16 to 100.
	- Dimensions should be as small as possible; many linear operations take place on this number.
		Six is a reasonable upper bound.
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
		if(MinimumFactor*2 > MaximumFactor) Message(Fatal, "RTree: MinimumFactor must be at most half of MaximumFactor.");
	}
	
	~RTree() {
		
	}
	
	void search(const BoundType &bound, SearchVisitorType &visitor)
		{ searchHelper(bound, m_root, visitor); }
	
	void insert(const BoundType &bound, const DataType &data) {
		if(m_root == NULL) {
			LeafNodeType *leafRoot;
			AesalonPoolAlloc(LeafNodeType, leafRoot, LeafNodeType());
			leafRoot->addBranch(bound, data);
			m_root = leafRoot;
			return;
		}
		
		NodeType *node = insertChooseLeaf(bound);
		
		bool succeeded = node->asLeafNode()->addBranch(bound, data);
		if(succeeded) return;
		LeafNodeType *newLeaf = splitNode(node->asLeafNode());
		
		NodeType *newNode = newLeaf;
		while(node != m_root) {
			InternalNodeType *parent = node->parent()->asInternalNode();
			
			int branch = parent->branch(node);
			parent->setBranchBound(branch, node->overallBound());
			
			if(newNode != NULL) {
				succeeded = parent->addBranch(newNode->overallBound(), newNode);
				if(!succeeded) {
					newNode = splitNode(parent);
				}
			}
			
			node = parent;
		}
		
		if(newNode != NULL) {
			InternalNodeType *newRoot;
			AesalonPoolAlloc(InternalNodeType, newRoot, InternalNodeType());
			newRoot->setDepth(newNode->depth()+1);
			newRoot->addBranch(node->overallBound(), node);
			newRoot->addBranch(newNode->overallBound(), newNode);
			m_root = newRoot;
		}
		
		//Message(Fatal, "Splitting support in insert NYI.");
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
				if(i == 0 || coverCost < minCost) minElement = i, minCost = coverCost;
			}
			
			node = node->asInternalNode()->branch(minElement);
			//Message(Fatal, "Insertion into non-root nodes NYI.");
		}
		return node;
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
			
			KeyType sep = highestStart[d] - lowestEnd[d];
			FloatKeyType sepNormalized = FloatKeyType(sep) / size;
			
			if(d == 0 || sepNormalized > greatestSeparation) {
				greatestSeparation = sepNormalized;
				greatestSeparationDimension = d;
			}
		}
		
		if(greatestSeparationDimension == -1) {
			Message(Fatal, "R-tree degeneracy case: all nodes have the same range. Cannot decide which to use as seeds for node splitting.");
		}
		
		seeds[0] = highestStart[greatestSeparationDimension];
		seeds[1] = lowestEnd[greatestSeparationDimension];
	}
	
	template<typename SplitNodeType>
	SplitNodeType *splitNode(SplitNodeType *toSplit) {
		int seeds[2];
		linearSplitSeeds(toSplit, seeds);
		
		BoundType tsBound;
		BoundType nBound;
		
		int tsSeed = 0;
		
		SplitNodeType *newNode;
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
			Message(Fatal, "Both seeds for node-splitting are identical. Fix linearSplitSeeds.");
		}
		
		/* Iterate through branches. */
		for(int i = toSplit->branchCount()-1; i >= 0; i --) {
			/* Skip the seed. */
			if(i == tsSeed) continue;
			
			int adjI = i;
			if(i > tsSeed) adjI --;
			
			const BoundType &bound = toSplit->branchBound(i);
			
			/* Handle the minumum branching factor cases . . . */
			if(toSplit->branchCount() == MinimumFactor) {
				Message(Debug, "toSplit minimum factor . . .");
				break;
			}
			else if(newNode->branchCount() + adjI == MinimumFactor) {
				Message(Debug, "newNode minimum factor . . .");
				Message(Warning, "TODO: Handle newNode minimum factor!");
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
			/* The other case is a fall-through. */
		}
		
		Message(Debug, "After splitting (toSplit/newNode) is " << toSplit->branchCount() << "/" << newNode->branchCount());
		
		return newNode;
	}
	
	InternalNodeType *splitNode(InternalNodeType *toSplit) {
		return splitNode<InternalNodeType>(toSplit);
	}
	
	LeafNodeType *splitNode(LeafNodeType *toSplit) {
		return splitNode<LeafNodeType>(toSplit);
	}
};

} // namespace Storage

#endif
