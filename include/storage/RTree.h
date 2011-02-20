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
	Note that Value must be either a basic type or a pointer.
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
		
		void setRange(Range &range, int dimension) { m_ranges[dimension] = range; }
		Range &range(int dimension) { return m_ranges[dimension]; }
		const Range &range(int dimension) const { return m_ranges[dimension]; }
		
		bool overlaps(const Bound &other) {
			for(int i = 0; i < Dimensions; i ++) {
				if(!m_ranges[i].overlaps(other.m_ranges[i])) return false;
			}
			return true;
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
		
		Branch &branch(int which) { return m_branches[which]; }
		int branchCount() const { return m_branchCount; }
		void setBranchCount(int count) { m_branchCount = count; }
		Node *parent() const { return m_parent; }
		void setParent(Node *node) { m_parent = node; }
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
		Branch b;
		b.bound = bound;
		b.value = value;
		ll = splitNode(leaf, b);
	}
	
	Message(Debug, "Calling adjustTree() . . .");
	adjustTree(leaf, ll);
	
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
			
			Message(Debug, "Considering branch #" << i);
			if(branch.bound.overlaps(bound)) {
				Message(Debug, "Branch overlaps bound, processing . . .");
				processor->process(branch.bound, branch.value);
			}
			else Message(Debug, "Branch does not overlap bound, ignoring.");
		}
	}
	else {
		for(int i = 0; i < node->branchCount(); i ++) {
			Branch &branch = node->branch(i);
			if(branch.bound.overlaps(bound)) search(bound, branch.node, processor);
		}
	}
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum>
typename RTree<Key, Value, Dimensions, Maximum, Minimum>::Node *
	RTree<Key, Value, Dimensions, Maximum, Minimum>::chooseLeaf(
		const RTree<Key, Value, Dimensions, Maximum, Minimum>::Bound &bound) {
	
	Node *node = m_root;
	
	while(!node->isLeaf()) {
		Node *smallestNode = NULL;
		Key smallestVolume = 0;
		for(int i = 0; i < node->branchCount(); i ++) {
			Key volume = 0;
			
			Bound &nbound = node->branch(i).bound;
			
			for(int j = 0; j < Dimensions; j ++) {
				Key difference = nbound.range(j).enlargementToCover(bound.range(j));
				if(difference == 0) continue;
				
				for(int k = 0; k < Dimensions; k ++) {
					if(k == j) continue;
					difference *= nbound.range(j).size();
				}
				volume += difference;
			}
			
			if(volume < smallestVolume || smallestNode == NULL) smallestNode = node, smallestVolume = volume;
			else if(volume == smallestVolume) {
				/* TODO: implement tiebreaker. */
				Message(Debug, "Tie-breaker not implemented!");
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
	
	while(n != m_root) {
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
	int listSize = Maximum-1;
	
	for(int i = 0; i < Maximum; i ++) {
		list[i] = node->branch(i);
	}
	list[Maximum] = branch;
	
	Key lowestValues[Dimensions];
	Key highestValues[Dimensions];
	Branch lowestBranches[Dimensions];
	Branch highestBranches[Dimensions];
	
	for(int i = 0; i < Dimensions; i ++) {
		for(int j = 0; j < Maximum+1; j ++) {
			if(lowestValues[i] > list[j].bound.range(i).start()) {
			}
			//lowestValues[i] = std::min(lowestValues[i], list[j].bound.range(i).start());
			//highestValues[i] = std::max(highestValues[i], list[j].bound.range(i).end());
		}
	}
	
	while(true) {
		
		// PickNext algorithm for linear-time is to simply pick any. Choose the first.
		
	}
	
	node = NULL;
	branch.node = NULL;
	
	return NULL;
}

} // namespace Storage

#endif
