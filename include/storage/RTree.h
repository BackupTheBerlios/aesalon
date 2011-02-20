/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/storage/RTree.h
*/

#ifndef AesalonStorage_RTree_H
#define AesalonStorage_RTree_H

#include <vector>

namespace Storage {

/** An n-dimensional R-tree.
	Note that Value must be either a basic type or a pointer.
*/
template<typename Key, typename Value, int Dimensions, int Maximum = 8, int Minimum = Maximum/2>
class RTree {
public:
	class Range {
	private:
		Key m_start, m_end;
	public:
		Range(Key start = 0, Key end = 0) : m_start(start), m_end(end) {}
		
		Key start() const { return m_start; }
		Key end() const { return m_end; }
		Key size() const { return m_end - m_start; }
		
		bool overlaps(const Range &other) const {
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
		
		bool overlaps(const Bound &other) {
			for(int i = 0; i < Dimensions; i ++) {
				if(!m_ranges[i].overlaps(other.m_ranges[i])) return false;
			}
			return true;
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
	public:
		Node(bool isLeaf) : m_isLeaf(isLeaf) {}
		bool isLeaf() const { return m_isLeaf; }
		
		Branch &branch(int which) { return m_branches[which]; }
		int branchCount() const { return m_branchCount; }
		void setBranchCount(int count) { m_branchCount = count; }
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
	Node *splitNode(Node *node, const Bound &bound, Value value);
	
	Node *adjustTree(Node *l, Node *ll);
};

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum>
RTree<Key, Value, Dimensions, Maximum, Minimum>::RTree() {
	m_root = NULL;
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum>
RTree<Key, Value, Dimensions, Maximum, Minimum>::~RTree() {
	
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum>
void RTree<Key, Value, Dimensions, Maximum, Minimum>::search(
	const RTree<Key, Value, Dimensions, Maximum, Minimum>::Bound &bound,
	RTree<Key, Value, Dimensions, Maximum, Minimum>::SearchProcessor *processor) {
	
	search(bound, m_root, processor);
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum>
void RTree<Key, Value, Dimensions, Maximum, Minimum>::insert(
	const RTree<Key, Value, Dimensions, Maximum, Minimum>::Bound &bound,
	Value value) {
	
	Node *leaf = chooseLeaf(bound), *ll = NULL;
	int count = leaf->branchCount();
	if(count < Maximum) {
		leaf->branch(count).value = value;
		leaf->setBranchCount(count+1);
	}
	else {
		ll = splitNode(leaf, bound, value);
	}
	
	Node *r = adjustTree(leaf, ll);
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum>
void RTree<Key, Value, Dimensions, Maximum, Minimum>::search(
	const RTree<Key, Value, Dimensions, Maximum, Minimum>::Bound &bound,
	RTree<Key, Value, Dimensions, Maximum, Minimum>::Node *node,
	RTree<Key, Value, Dimensions, Maximum, Minimum>::SearchProcessor *processor) {
	
	if(node == NULL) return;
	
	if(node->isLeaf()) {
		for(int i = 0; i < node->branchCount(); i ++) {
			Branch &branch = node->branch(i);
			if(branch.bound.overlaps(bound)) processor->process(branch.bound, branch.value);
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
		}
		
		/* TODO: implement tiebreaker. */
		
		node = smallestNode;
	}
	
	return node;
}



} // namespace Storage

#endif
