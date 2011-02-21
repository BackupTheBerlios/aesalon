/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/storage/RTree.h
*/

#ifndef AesalonStorage_RTree_H
#define AesalonStorage_RTree_H

#include <vector>

#include "util/MessageSystem.h"

namespace Storage {

/** An n-dimensional R-tree.
	Notes:
	- Key must support a basic assignment specifier and also the element zero, plus all the usual arithmetic operators.
		Using a non-elementary type is not a good idea.
	- Value must be either a basic type or a pointer.
	- Minimum <= Maximum/2. (Logic in code assumes this.)
	- Maximum <= some reasonable number. Many linear searches through lists of this size take place.
		A reasonable size is eight.
	- Dimensions should be less than about six. Any more and you have other problems . . .
*/
template<typename Key, typename Value, int Dimensions, int Maximum = 8, int Minimum = Maximum/2>
class RTree {
public:
	class Range {
	private:
		bool m_valid;
		Key m_start, m_end;
	public:
		Range() : m_valid(false) {}
		Range(Key start, Key end) : m_valid(true), m_start(start), m_end(end) {}
		
		bool valid() const { return m_valid; }
		Key start() const { return m_start; }
		Key end() const { return m_end; }
		Key size() const { return m_end - m_start; }
		
		bool overlaps(const Range &other) const {
			Message(Debug, "Overlap test:");
			Message(Debug, "\tm_start: " << m_start << "\t m_end: " << m_end);
			Message(Debug, "\tother.m_start: " << other.m_start << "\tother.m_end: " << other.m_end);
			if(m_start > other.m_end || other.m_start > m_end) return false;
			return true;
		}
		
		Key enlargementToCover(const Range &other) const {
			Key sum = 0;
			if(m_start > other.m_start) sum += (m_start - other.m_start);
			if(m_end < other.m_end) sum += (other.m_end - m_end);
			return sum;
		}
	};
	
	class Bound {
	private:
		Range m_ranges[Dimensions];
	public:
		Bound() {}
		Bound(Range ranges[Dimensions]) {
			for(int i = 0; i < Dimensions; i ++) m_ranges[i] = ranges[i];
		}
		
		void setRange(const Range &range, int dimension) { m_ranges[dimension] = range; }
		Range &range(int dimension) { return m_ranges[dimension]; }
		const Range &range(int dimension) const { return m_ranges[dimension]; }
		
		bool overlaps(const Bound &other) {
			for(int i = 0; i < Dimensions; i ++) {
				if(!m_ranges[i].overlaps(other.m_ranges[i])) return false;
			}
			return true;
		}
		
		Key enlargementToCover(const Bound &other) const {
			Key volume = 0;
			
			for(int j = 0; j < Dimensions; j ++) {
				Key difference = other.m_ranges[j].enlargementToCover(m_ranges[j]);
				if(difference == 0) continue;
				
				for(int k = 0; k < Dimensions; k ++) {
					if(k == j) continue;
					difference *= other.m_ranges[j].size();
				}
				volume += difference;
			}
			
			return volume;
		}
		
		void enclose(const Bound &other) {
			for(int i = 0; i < Dimensions; i ++) {
				if(m_ranges[i].valid() && other.m_ranges[i].valid()) {
					m_ranges[i] = Range(
						std::min(m_ranges[i].start(), other.m_ranges[i].start()),
						std::max(m_ranges[i].end(), other.m_ranges[i].end()));
				}
				else if(other.m_ranges[i].valid()) {
					m_ranges[i] = other.m_ranges[i];
				}
				// If other.m_ranges[i] is not valid, nothing needs to be done.
			}
		}
	};
	
	class SearchProcessor {
	public:
		virtual bool process(const Bound &bound, Value value) = 0;
	};
protected:
	class Node;
	struct Branch {
		Bound bound;
		union {
			Node *node;
			Value value;
		};
	};
	class Node {
	private:
		bool m_isLeaf;
		Branch m_branches[Maximum];
		int m_branchCount;
		Node *m_parent;
	public:
		Node(bool isLeaf) : m_isLeaf(isLeaf), m_branchCount(0), m_parent(NULL) {}
		bool isLeaf() const { return m_isLeaf; }
		void setLeaf(bool leaf) { m_isLeaf = leaf; }
		
		Branch &branch(int which) { return m_branches[which]; }
		int branchCount() const { return m_branchCount; }
		void setBranchCount(int count) { m_branchCount = count; }
		Node *parent() const { return m_parent; }
		void setParent(Node *node) { m_parent = node; }
		
		Bound bound() const {
			Bound result;
			for(int i = 0; i < Dimensions; i ++) {
				Key minimum, maximum;
				for(int j = 0; j < branchCount(); j ++) {
					if(j == 0) {
						minimum = m_branches[j].bound.range(i).start();
						maximum = m_branches[j].bound.range(i).end();
						continue;
					}
					minimum = std::min(minimum, m_branches[j].bound.range(i).start());
					maximum = std::max(maximum, m_branches[j].bound.range(i).end());
				}
				result.range(i) = Range(minimum, maximum);
			}
			return result;
		}
	};
private:
	Node *m_root;
public:
	RTree();
	~RTree();
	
	void search(const Bound &bound, SearchProcessor *processor);
	void insert(const Bound &bound, Value value);
private:
	void search(const Bound &bound, Node *node, SearchProcessor *processor);
	
	Node *chooseLeaf(const Bound &bound);
	
	Node *adjustTree(Node *n, Node *nn);
	
	Node *splitNode(Node *node, Branch &branch);
};

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum>
RTree<Key, Value, Dimensions, Maximum, Minimum>::RTree() {
	m_root = new Node(true);
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum>
RTree<Key, Value, Dimensions, Maximum, Minimum>::~RTree() {
	
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum>
void RTree<Key, Value, Dimensions, Maximum, Minimum>::search(
	const RTree<Key, Value, Dimensions, Maximum, Minimum>::Bound &bound,
	RTree<Key, Value, Dimensions, Maximum, Minimum>::SearchProcessor *processor) {
	
	Message(Debug, "Beginning search.");
	
	search(bound, m_root, processor);
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum>
void RTree<Key, Value, Dimensions, Maximum, Minimum>::insert(
	const RTree<Key, Value, Dimensions, Maximum, Minimum>::Bound &bound,
	Value value) {
	
	Message(Debug, "Beginning insertion of value " << value);
	
	Message(Debug, "Calling chooseLeaf() . . .");
	Node *leaf = chooseLeaf(bound), *ll = NULL;
	int count = leaf->branchCount();
	if(count < Maximum) {
		Message(Debug, "Leaf node count is less than the maximum, inserting as usual.");
		leaf->branch(count).bound = bound;
		leaf->branch(count).value = value;
		leaf->setBranchCount(count+1);
		Message(Debug, "Leaf node new branch count: " << leaf->branchCount());
	}
	else {
		Message(Debug, "Leaf node branch count is already at maximum, splitting node . . .");
		Branch b;
		b.bound = bound;
		b.value = value;
		ll = splitNode(leaf, b);
		Message(Debug, "Node split, ll is " << ll);
	}
	
	Message(Debug, "Calling adjustTree() . . .");
	Node *result = adjustTree(leaf, ll);
	if(result != NULL) {
		Message(Debug, "**** Creating new root node!");
		Node *newRoot = new Node(false);
		newRoot->setBranchCount(2);
		newRoot->branch(0).bound = m_root->bound();
		newRoot->branch(0).node = m_root;
		newRoot->branch(1).bound = result->bound();
		newRoot->branch(1).node = result;
		m_root->setParent(newRoot);
		result->setParent(newRoot);
		m_root = newRoot;
	}
	
	Message(Debug, "Finished.");
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum>
void RTree<Key, Value, Dimensions, Maximum, Minimum>::search(
	const RTree<Key, Value, Dimensions, Maximum, Minimum>::Bound &bound,
	RTree<Key, Value, Dimensions, Maximum, Minimum>::Node *node,
	RTree<Key, Value, Dimensions, Maximum, Minimum>::SearchProcessor *processor) {
	
	if(node == NULL) return;
	
	if(node->isLeaf()) {
		Message(Debug, "At leaf node! Branch count: " << node->branchCount());
		for(int i = 0; i < node->branchCount(); i ++) {
			Branch &branch = node->branch(i);
			
			Message(Debug, "Considering leaf branch #" << i);
			if(branch.bound.overlaps(bound)) {
				Message(Debug, "Leaf branch overlaps bound, processing . . .");
				processor->process(branch.bound, branch.value);
			}
			else Message(Debug, "Leaf branch does not overlap bound, ignoring.");
		}
	}
	else {
		Message(Debug, "Not in leaf node. Branch count: " << node->branchCount());
		for(int i = 0; i < node->branchCount(); i ++) {
			Branch &branch = node->branch(i);
			Message(Debug, "Considering non-leaf branch #" << i);
			Message(Debug, "\tbranchCount():" << branch.node->branchCount());
			if(branch.bound.overlaps(bound)) {
				Message(Debug, "Non-leaf branch overlaps bound, processing . . .");
				search(bound, branch.node, processor);
			}
			else {
				Message(Debug, "Non-leaf branch does not overlap bound, ignoring.");
			}
		}
	}
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum>
typename RTree<Key, Value, Dimensions, Maximum, Minimum>::Node *
	RTree<Key, Value, Dimensions, Maximum, Minimum>::chooseLeaf(
		const RTree<Key, Value, Dimensions, Maximum, Minimum>::Bound &bound) {
	
	Node *node = m_root;
	
	while(!node->isLeaf()) {
		Message(Debug, "Node is not leaf node, continuing downwards . . .");
		Node *smallestNode = NULL;
		Key smallestVolume = 0;
		for(int i = 0; i < node->branchCount(); i ++) {
			Key volume = bound.enlargementToCover(node->branch(i).bound);
			
			if(volume < smallestVolume || smallestNode == NULL) {
				smallestNode = node->branch(i).node;
				smallestVolume = volume;
			}
			else if(volume == smallestVolume) {
				/* TODO: implement tiebreaker. */
				Message(Debug, "RTree: smallestVolume tie-breaker not implemented!");
			}
		}
		
		node = smallestNode;
	}
	
	Message(Debug, "Found leaf node.");
	Message(Debug, "Leaf node branch count: " << node->branchCount());
	
	return node;
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum>
typename RTree<Key, Value, Dimensions, Maximum, Minimum>::Node *
	RTree<Key, Value, Dimensions, Maximum, Minimum>::adjustTree(
		RTree<Key, Value, Dimensions, Maximum, Minimum>::Node *n,
		RTree<Key, Value, Dimensions, Maximum, Minimum>::Node *nn) {
	
	Message(Debug, "Starting tree adjustment, nn is " << nn);
	
	while(n != m_root) {
		Message(Debug, "*** Adjusting tree node . . .");
		Node *p = n->parent();
		
		int i = 0;
		for(; i < p->branchCount(); i ++) {
			if(p->branch(i).node == n) {
				break;
			}
		}
		
		Bound &bound = p->branch(i).bound;
		
		for(; i < n->branchCount(); i ++) {
			bound.enclose(p->branch(i).bound);
		}
		
		if(nn != NULL) {
			// Add nn to p.
			int count = p->branchCount();
			if(count < Maximum) {
				for(i = 0; i < nn->branchCount(); i ++) {
					p->branch(count).bound.enclose(nn->branch(i).bound);
				}
				p->branch(count).node = nn;
				nn = NULL;
			}
			else {
				// Invoke splitNode. TBI.
				Message(Fatal, "Splitting nodes in adjustTree NYI.");
				//nn = splitNode(n);
			}
		}
		
		n = p;
	}
	
	return nn;
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum>
typename RTree<Key, Value, Dimensions, Maximum, Minimum>::Node *
	RTree<Key, Value, Dimensions, Maximum, Minimum>::splitNode(
		RTree<Key, Value, Dimensions, Maximum, Minimum>::Node *node,
		RTree<Key, Value, Dimensions, Maximum, Minimum>::Branch &branch) {
	
	// Linear-time algorithm specified in original R-tree publication.
	
	Node *nn = new Node(node->isLeaf());
	nn->setParent(node->parent());
	
	Branch list[Maximum+1];
	int listSize = Maximum+1;
	
	for(int i = 0; i < Maximum; i ++) {
		list[i] = node->branch(i);
	}
	list[Maximum] = branch;
	
	node->setBranchCount(0);
	nn->setBranchCount(0);
	
	int highestStart[Dimensions];
	int lowestEnd[Dimensions];
	Key lowest[Dimensions];
	Key highest[Dimensions];
	
	for(int j = 0; j < Maximum+1; j ++) {
		for(int i = 0; i < Dimensions; i ++) {
			const Range &iRange = list[j].bound.range(i);
			if(j == 0) {
				lowest[i] = iRange.start();
				highest[i] = iRange.end();
				highestStart[i] = 0;
				lowestEnd[i] = 0;
				continue;
			}
			lowest[i] = std::min(lowest[i], iRange.start());
			highest[i] = std::max(highest[i], iRange.end());
			
			Range &itemRange = list[j].bound.range(i);
			if(list[highestStart[i]].bound.range(i).start() > itemRange.start()) {
				highestStart[i] = j;
			}
			if(list[lowestEnd[i]].bound.range(i).end() < itemRange.end()) {
				lowestEnd[i] = j;
			}
		}
	}
	
	Key maxSeparation = 0;
	int maxIndex = -1;
	
	/* Create normalized separations. */
	for(int i = 0; i < Dimensions; i ++) {
		Key separation = 
			(list[lowestEnd[i]].bound.range(i).end() - list[highestStart[i]].bound.range(i).start())
			/ (highest[i]-lowest[i]);
		
		if(maxIndex == -1 || separation > maxSeparation) maxIndex = i, maxSeparation = separation;
	}
	
	node->branch(0) = list[highestStart[maxIndex]];
	node->setBranchCount(1);
	nn->branch(0) = list[lowestEnd[maxIndex]];
	nn->setBranchCount(1);
	
	Bound nodeBound = list[highestStart[maxIndex]].bound;
	Bound nnBound = list[lowestEnd[maxIndex]].bound;
	
	/* Do the removal in the correct order . . . */
	if(highestStart[maxIndex] > lowestEnd[maxIndex]) {
		list[highestStart[maxIndex]] = list[listSize-1];
		list[lowestEnd[maxIndex]] = list[listSize-2];
		listSize -= 2;
	}
	else if(lowestEnd[maxIndex] > highestStart[maxIndex]) {
		list[lowestEnd[maxIndex]] = list[listSize-1];
		list[highestStart[maxIndex]] = list[listSize-2];
		listSize -= 2;
	}
	/* They are one and the same . . . this should not happen! */
	else {
		Message(Fatal, "R-tree: degeneracy case, all entries are the same. Support NYI.");
	}
	
	while(true) {
		if(listSize == 0) break;
		else if((node->branchCount() + listSize) == Minimum) {
			Message(Debug, "Node+listSize == Minimum, filling . . .");
			Message(Debug, "listSize: " << listSize);
			Message(Debug, "Before: " << node->branchCount());
			for(int i = 0; i < listSize; i ++) {
				node->branch(node->branchCount()) = list[i];
				node->setBranchCount(node->branchCount()+1);
			}
			Message(Debug, "After: " << node->branchCount());
			break;
		}
		else if((nn->branchCount() + listSize) == Minimum) {
			Message(Debug, "nn+listSize == Minimum, filling . . .");
			for(int i = 0; i < listSize; i ++) {
				nn->branch(node->branchCount()) = list[i];
				nn->setBranchCount(nn->branchCount()+1);
			}
			break;
		}
		/* PickNext algorithm for linear-time is to simply pick any item. Choose the last. */
		Key volume1 = nodeBound.enlargementToCover(list[listSize-1].bound);
		Key volume2 = nnBound.enlargementToCover(list[listSize-1].bound);
		
		if(volume1 < volume2) {
			Message(Debug, "Adding to node . . .");
			node->branch(node->branchCount()) = list[listSize-1];
			node->setBranchCount(node->branchCount()+1);
			nodeBound.enclose(list[listSize-1].bound);
		}
		else if(volume2 < volume1) {
			Message(Debug, "Adding to nn . . .");
			nn->branch(node->branchCount()) = list[listSize-1];
			nn->setBranchCount(nn->branchCount()+1);
			nnBound.enclose(list[listSize-1].bound);
		}
		else {
			Message(Warning, "RTree: splitNode tie-breaker NYI. Defaulting to node.");
			node->branch(node->branchCount()) = list[listSize-1];
			node->setBranchCount(node->branchCount()+1);
			nodeBound.enclose(list[listSize-1].bound);
		}
		listSize --;
	}
	
	Message(Debug, "\tBefore splitting, there were " << Maximum+1 << " elements.");
	Message(Debug, "\tAfter splitting, there are " << node->branchCount() << " + " << nn->branchCount()
		<< " elements.");
	
	return nn;
}

} // namespace Storage

#endif
