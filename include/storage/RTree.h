/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/storage/RTree.h
*/

#ifndef AesalonStorage_RTree_H
#define AesalonStorage_RTree_H

#include <vector>
#include <list>
#include <queue>

#include "util/MessageSystem.h"

namespace Storage {

/** An n-dimensional R-tree.
	Notes:
	- Key must support a basic assignment specifier and also the element zero, plus all the usual arithmetic operators.
		Using an elementary type is a very good idea.
	- Value must be either a basic type or a pointer.
	- Minimum <= Maximum/2. (Logic in code assumes this.)
	- Maximum <= some reasonable number. Many linear searches through lists of this size take place.
		A reasonable size is sixteen.
	- Dimensions should be as small as possible; many linear operations take place on this number.
		Six is a reasonable upper bound.
	- FloatKey should be a version of Key that supports floating-point arithmetic (or at least non-integer division).
	Unless otherwise noted, all algorithms are from
		A Guttman, R-Trees: A Dynamic Index Structure for Spatial Searching, 1984.
	
	@todo Improve condenseTree's handling of non-leaf nodes.
*/
template<typename Key, typename Value, int Dimensions, int Maximum = 8, int Minimum = Maximum/2,
	typename FloatKey = Key>
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
			if(m_start > other.m_end || other.m_start > m_end) {
				return false;
			}
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
		
		bool overlaps(const Bound &other) const {
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
				/* If other.m_ranges[i] is not valid, nothing needs to be done. */
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
				Key minimum = 0, maximum = 0;
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
	
	/** Searches the tree for anything overlapping bound.
		@param bound The bound to search for.
		@param processor A search processor.
	*/
	void search(const Bound &bound, SearchProcessor *processor);
	/** Inserts a value into the tree with a bound.
		@param bound The bound to use.
		@param value The value to insert.
	*/
	void insert(const Bound &bound, Value value);
	/** Removes a value from the tree.
		@param bound The bound to search.
		@param value The value to remove.
	*/
	void remove(const Bound &bound, Value value);
	
	/** Returns the overall bounds of the tree.
		@return The overall bounds of the tree.
	*/
	Bound bounds();
private:
	void search(const Bound &bound, Node *node, SearchProcessor *processor);
	
	Node *chooseLeaf(const Bound &bound);
	Node *adjustTree(Node *n, Node *nn);
	Node *splitNode(Node *node, Branch &branch);
	
	Node *findLeaf(Node *node, const Bound &bound, Value value);
	void condenseTree(Node *leaf);
};

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum,
	typename FloatKey>
RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::RTree() {
	m_root = new Node(true);
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum,
	typename FloatKey>
RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::~RTree() {
	
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum,
	typename FloatKey>
void RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::search(
	const RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::Bound &bound,
	RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::SearchProcessor *processor) {
	
	search(bound, m_root, processor);
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum,
	typename FloatKey>
void RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::insert(
	const RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::Bound &bound,
	Value value) {
	
	Node *leaf = chooseLeaf(bound), *ll = NULL;
	int count = leaf->branchCount();
	if(count < Maximum) {
		leaf->branch(count).bound = bound;
		leaf->branch(count).value = value;
		leaf->setBranchCount(count+1);
	}
	else {
		Branch b;
		b.bound = bound;
		b.value = value;
		ll = splitNode(leaf, b);
	}
	
	Node *result = adjustTree(leaf, ll);
	if(result != NULL) {
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
	
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum,
	typename FloatKey>
void RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::remove(
	const RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::Bound &bound,
	Value value) {
	
	Node *leaf = findLeaf(m_root, bound, value);
	
	condenseTree(leaf);
	
	if(!m_root->isLeaf() && m_root->branchCount() == 1) {
		Node *newRoot = m_root->branch(0).node;
		newRoot->setParent(NULL);
		delete m_root;
		m_root = newRoot;
	}
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum,
	typename FloatKey>
typename RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::Bound
	RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::bounds() {
	
	return m_root->bound();
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum,
	typename FloatKey>
void RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::search(
	const RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::Bound &bound,
	RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::Node *node,
	RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::SearchProcessor *processor) {
	
	if(node == NULL) return;
	
	if(node->isLeaf()) {
		for(int i = 0; i < node->branchCount(); i ++) {
			Branch &branch = node->branch(i);
			
			if(branch.bound.overlaps(bound)) {
				processor->process(branch.bound, branch.value);
			}
		}
	}
	else {
		for(int i = 0; i < node->branchCount(); i ++) {
			Branch &branch = node->branch(i);
			if(branch.bound.overlaps(bound)) {
				search(bound, branch.node, processor);
			}
		}
	}
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum,
	typename FloatKey>
typename RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::Node *
	RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::chooseLeaf(
		const RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::Bound &bound) {
	
	Node *node = m_root;
	
	while(!node->isLeaf()) {
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
				//Message(Debug, "RTree: smallestVolume tie-breaker not implemented!");
			}
		}
		
		node = smallestNode;
	}
	
	return node;
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum,
	typename FloatKey>
typename RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::Node *
	RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::adjustTree(
		RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::Node *n,
		RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::Node *nn) {
	
	while(n != m_root) {
		Node *p = n->parent();
		
		int i = 0;
		for(; i < p->branchCount(); i ++) {
			if(p->branch(i).node == n) {
				break;
			}
		}
		
		p->branch(i).bound = n->bound();
		
		if(nn != NULL) {
			/* Add nn to p. */
			int count = p->branchCount();
			if(count < Maximum) {
				p->branch(count).bound = nn->bound();
				p->branch(count).node = nn;
				nn = NULL;
				p->setBranchCount(p->branchCount()+1);
			}
			else {
				/* Invoke splitNode to create pp. */
				
				Branch b;
				b.bound = nn->bound();
				b.node = nn;
				
				nn = splitNode(p, b);
			}
		}
		
		n = p;
	}
	
	return nn;
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum,
	typename FloatKey>
typename RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::Node *
	RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::splitNode(
		RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::Node *node,
		RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::Branch &branch) {
	
	/* This uses the linear-time algorithm specified in the publication. */
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
	
	int highestStart[Dimensions] = {0};
	int lowestEnd[Dimensions] = {0};
	Key lowest[Dimensions] = {0};
	Key highest[Dimensions] = {0};
	
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
		Key separation;
		if(highest[i]-lowest[i] == 0) separation = 0;
		else separation = 
			(list[lowestEnd[i]].bound.range(i).end() - list[highestStart[i]].bound.range(i).start())
			/ (highest[i]-lowest[i]);
		
		if(maxIndex == -1 || separation > maxSeparation) maxIndex = i, maxSeparation = separation;
	}
	
	Bound nodeBound;
	Bound nnBound;
	
	if(highestStart[maxIndex] != lowestEnd[maxIndex]) {
		node->branch(0) = list[highestStart[maxIndex]];
		node->setBranchCount(1);
		nn->branch(0) = list[lowestEnd[maxIndex]];
		nn->setBranchCount(1);
	
		nodeBound = list[highestStart[maxIndex]].bound;
		nnBound = list[lowestEnd[maxIndex]].bound;
		
		/* Do the removal in the correct order . . . */
		if(highestStart[maxIndex] > lowestEnd[maxIndex]) {
			list[highestStart[maxIndex]] = list[listSize-1];
			list[lowestEnd[maxIndex]] = list[listSize-2];
		}
		else if(lowestEnd[maxIndex] > highestStart[maxIndex]) {
			list[lowestEnd[maxIndex]] = list[listSize-1];
			list[highestStart[maxIndex]] = list[listSize-2];
		}
	}
	/* They are one and the same . . . this only happens if all ranges are identical in this node.
		Thus, any elements will do perfectly well.
	*/
	else {
		node->branch(0) = list[listSize-1];
		node->setBranchCount(1);
		nn->branch(0) = list[listSize-2];
		nn->setBranchCount(1);
		
		nodeBound = list[listSize-1].bound;
		nnBound = list[listSize-2].bound;
	}
	listSize -= 2;
	
	while(true) {
		if(listSize == 0) break;
		else if((node->branchCount() + listSize) == Minimum) {
			for(int i = 0; i < listSize; i ++) {
				node->branch(node->branchCount()) = list[i];
				node->setBranchCount(node->branchCount()+1);
			}
			break;
		}
		else if((nn->branchCount() + listSize) == Minimum) {
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
			node->branch(node->branchCount()) = list[listSize-1];
			node->setBranchCount(node->branchCount()+1);
			nodeBound.enclose(list[listSize-1].bound);
		}
		else if(volume2 < volume1) {
			nn->branch(nn->branchCount()) = list[listSize-1];
			nn->setBranchCount(nn->branchCount()+1);
			nnBound.enclose(list[listSize-1].bound);
		}
		else {
			//Message(Warning, "RTree: splitNode tie-breaker NYI. Defaulting to node.");
			node->branch(node->branchCount()) = list[listSize-1];
			node->setBranchCount(node->branchCount()+1);
			nodeBound.enclose(list[listSize-1].bound);
		}
		listSize --;
	}
	
	return nn;
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum,
	typename FloatKey>
typename RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::Node *
	RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::findLeaf(
		RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::Node *node,
		const RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::Bound &bound,
		Value value) {
	
	if(node->isLeaf()) {
		for(int i = 0; i < node->branchCount(); i ++) {
			if(node->branch(i).value == value) {
				node->branch(i) = node->branch(node->branchCount()-1);
				node->setBranchCount(node->branchCount()-1);
				return node;
			}
		}
		return NULL;
	}
	else {
		for(int i = 0; i < node->branchCount(); i ++) {
			if(bound.overlaps(node->branch(i).bound)) {
				Node *result = findLeaf(node->branch(i).node, bound, value);
				if(result != NULL) return result;
			}
		}
	}
	return NULL;
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum,
	typename FloatKey>
void RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::condenseTree(
		RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>::Node *leaf) {
	
	Node *n = leaf;
	std::list<Node *> removedNodes;
	
	while(m_root != n) {
		Node *p = n->parent();
		
		int i;
		for(i = 0; i < p->branchCount(); i ++) {
			if(p->branch(i).node == n) break;
		}
		
		if(n->branchCount() < Minimum) {
			p->branch(i) = p->branch(p->branchCount()-1);
			p->setBranchCount(p->branchCount()-1);
			removedNodes.push_back(n);
		}
		else {
			p->branch(i).bound = n->bound();
		}
		n = p;
	}
	
	Message(Debug, "Reinserting " << removedNodes.size() << " node(s) into tree.");
	
	for(typename std::list<Node *>::iterator i = removedNodes.begin(); i != removedNodes.end(); ++i) {
		Node *node = *i;
		
		if(node->isLeaf()) {
			for(int i = 0; i < node->branchCount(); i ++) {
				Branch &b = node->branch(i);
				insert(b.bound, b.value);
			}
		}
		/* TODO: implement better support for removal. This is horribly slow. */
		else {
			std::queue<Node *> q;
			q.push(node);
			while(q.size()) {
				node = q.front();
				q.pop();
				if(node->isLeaf()) {
					for(int i = 0; i < node->branchCount(); i ++) {
						Branch &b = node->branch(i);
						insert(b.bound, b.value);
					}
				}
				else {
					for(int i = 0; i < node->branchCount(); i ++) {
						q.push(node->branch(i).node);
					}
				}
			}
		}
	}
	
	
}

} // namespace Storage

#endif
