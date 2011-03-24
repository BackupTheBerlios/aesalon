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

#define RTreeTemplate \
	template< RTreeTemplateItems >
#define RTreeTemplateItems \
	typename Key, typename Value, int Dimensions, int Maximum, int Minimum, typename FloatKey
#define RTreeScope \
	RTree<Key, Value, Dimensions, Maximum, Minimum, FloatKey>

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum, typename FloatKey=Key>
class RTree {
public:
	class Bound {
	private:
		Key m_bounds[Dimensions][2];
	public:
		Bound() {
			for(int i = 1; i < Dimensions; i ++) {
				m_bounds[i][0] = m_bounds[i][1] = 0.0;
			}
		}
		Bound(Key d1_start, Key d1_end, ...) {
			va_list ap;
			va_start(ap, d1_end);
			
			m_bounds[0][0] = d1_start, m_bounds[0][1] = d1_end;
			
			for(int i = 1; i < Dimensions; i ++) {
				m_bounds[i][0] = va_arg(ap, Key);
				m_bounds[i][1] = va_arg(ap, Key);
			}
			
			va_end(ap);
		}
		
		Key start(int dimension) const { return m_bounds[dimension][0]; }
		Key end(int dimension) const { return m_bounds[dimension][1]; }
		
		bool overlaps(const Bound &other) const {
			for(int i = 0; i < Dimensions; i ++) {
				if(m_bounds[i][0] > other.m_bounds[i][1] ||
					m_bounds[i][1] < other.m_bounds[i][0]) return false;
			}
			return true;
		}
		
		Key toCover(const Bound &other) {
			Key volume = 0;
			Key bounds[Dimensions][2];
			Key sizes[Dimensions];
			
			for(int i = 0; i < Dimensions; i ++) {
				bounds[i][0] = m_bounds[i][0];
				bounds[i][1] = m_bounds[i][1];
				sizes[i] = m_bounds[i][1] - m_bounds[i][0];
			}
			
			for(int i = 0; i < Dimensions; i ++) {
				Key delta = 0;
				if(other.m_bounds[i][0] < bounds[i][0]) delta += bounds[i][0] - other.m_bounds[i][0];
				if(other.m_bounds[i][1] > bounds[i][1]) delta += other.m_bounds[i][1] - bounds[i][1];
				sizes[i] += delta;
				
				for(int i = 0; i < Dimensions; i ++) {
					volume += delta*sizes[i];
				}
			}
			return volume;
		}
		
		void cover(const Bound &other) {
			for(int i = 0; i < Dimensions; i ++) {
				m_bounds[i][0] = std::min(m_bounds[i][0], other.m_bounds[i][0]);
				m_bounds[i][1] = std::max(m_bounds[i][1], other.m_bounds[i][1]);
			}
		}

		Key volume() const {
			Key value = m_bounds[0][1] - m_bounds[0][0];
			for(int i = 1; i < Dimensions && value != 0; i ++) {
				value *= (m_bounds[i][1] - m_bounds[i][0]);
			}
			return value;
		}
	};
	
	class Callback {
	public:
		virtual ~Callback() {}
		
		/** Handle a search result.
			@return True if the search should continue.
		*/
		virtual bool handle(const Bound &bound, Value &value) = 0;
	};
protected:
	class LeafNode;
	class InternalNode;
	
	class Node {
	protected:
		Bound m_bound;
		int m_depth;
		int m_branches;
		Node *m_parent;
	public:
		Node(int depth) : m_depth(depth), m_branches(0) {}
		virtual ~Node() {}
		
		Bound &bound() { return m_bound; }
		const Bound &bound() const { return m_bound; }
		virtual const Bound &bound(int which) = 0;
		
		int depth() const { return m_depth; }
		bool isLeaf() const { return m_depth == 0; }
		
		bool isFull() const { return m_branches == Maximum; }
		int branches() const { return m_branches; }
		
		Node *parent() const { return m_parent; }
		void setParent(Node *parent) { m_parent = parent; }
		
		LeafNode *asLeaf() { return dynamic_cast<LeafNode *>(this); }
		InternalNode *asInternal() { return dynamic_cast<InternalNode *>(this); }
	};
	
	class LeafNode : public Node {
	private:
		struct Branch {
			Bound bound;
			Value value;
		};
		
		Branch m_branch[Maximum];
	public:
		LeafNode() : Node(0) {}
		virtual ~LeafNode() {}
		
		void addBranch(const Bound &bound, Value value) {
			m_branch[this->m_branches].bound = bound;
			m_branch[this->m_branches++].value = value;
		}
		void removeBranch(int which) {
			if(this->m_branches == 0) return;
			m_branch[which] = m_branch[--this->m_branches];
		}
		Bound &bound() {
			return this->Node::bound();
		}
		virtual const Bound &bound(int which) {
			return m_branch[which].bound;
		}
		Value &value(int which) {
			return m_branch[which].value;
		}
	};
	
	class InternalNode : public Node {
	private:
		Node *m_branch[Maximum];
	public:
		InternalNode(int depth) : Node(depth) {}
		
		void addBranch(Node *node) {
			m_branch[this->m_branches++] = node;
			node->setParent(this);
		}
		void removeBranch(int which) {
			if(this->m_branches == 0) return;
			m_branch[which] = m_branch[--this->m_branches];
		}

		Bound &bound() {
			return this->Node::bound();
		}
		
		virtual const Bound &bound(int which) {
			return m_branch[which]->bound();
		}
		
		Node *node(int which) {
			return m_branch[which];
		}
	};
private:
	Node *m_root;
public:
	RTree();
	~RTree();
	
	void search(const Bound &bound, Callback *callback);
	void insert(const Bound &bound, const Value &value);
private:
	bool search(Node *node, const Bound &bound, Callback *callback);
	
	Node *splitLeaf(Node *node);
	Node *splitInternal(Node *node);
};

RTreeTemplate
RTreeScope::RTree() {
	m_root = NULL;
}

RTreeTemplate
RTreeScope::~RTree() {
	
}

RTreeTemplate
void RTreeScope::search(const RTreeScope::Bound &bound, RTreeScope::Callback *callback) {
	Message(Debug, "RTreeScope::search() called . . .");
	search(m_root, bound, callback);
}

RTreeTemplate
void RTreeScope::insert(const RTreeScope::Bound &bound, const Value &value) {
	/* Simple case . . . */
	if(m_root == NULL) {
		LeafNode *leaf;
		AesalonPoolAlloc(LeafNode, leaf, LeafNode());
		leaf->addBranch(bound, value);
		m_root = leaf;
		return;
	}
	//Node *toInsert;
	//AesalonPoolAlloc(Node, toInsert, Node(bound, value, NULL));
	
	Node *node = m_root;
	while(!node->isLeaf()) {
		node = node->asInternal()->node(0);
	}
	
	Node *nn = NULL;
	if(node->branches() == Maximum) {
		nn = splitLeaf(node);
		if(nn->bound().toCover(bound) < node->bound().toCover(bound)) {
			nn->asLeaf()->addBranch(bound, value);
		}
		else {
			node->asLeaf()->addBranch(bound, value);
		}
	}
	else {
		node->asLeaf()->addBranch(bound, value);
	}

	Message(Debug, "nn: " << nn);

	/* Adjust the tree, going upwards. */
	while(nn != NULL && node != m_root) {
		Node *parent = node->parent();

		Message(Debug, "parent: " << parent);
		Message(Debug, "m_root: " << m_root);
		Message(Debug, "node: " << node);
		
		if(parent->branches() == Maximum) {
			Message(Fatal, "Case NYI.");
		}
		else {
			parent->asInternal()->addBranch(nn);
			parent->bound().cover(nn->bound());
			nn = NULL;
		}
		node = parent;
	}

	if(nn != NULL) {
		InternalNode *newRoot;
		AesalonPoolAlloc(InternalNode, newRoot, InternalNode(m_root->depth()+1));

		newRoot->addBranch(m_root);
		newRoot->addBranch(nn);
		newRoot->bound().cover(m_root->bound());
		newRoot->bound().cover(m_root->bound());
		m_root = newRoot;
	}
}

RTreeTemplate
bool RTreeScope::search(RTreeScope::Node *node, const RTreeScope::Bound &bound, RTreeScope::Callback *callback) {
	Message(Debug, "**** Beginning search.");
	if(node == NULL) {
		Message(Debug, "NULL node . . .");
		return true;
	}
	
	for(int i = 0; i < node->branches(); i ++) {
		Message(Debug, "Searching node . . .");
		const Bound &nbound = node->bound(i);
		if(bound.overlaps(nbound)) {
			Message(Debug, "\tBound overlaps!");
			if(node->isLeaf()) {
				if(!callback->handle(nbound, node->asLeaf()->value(i))) return false;
			}
			else {
				if(!search(node->asInternal()->node(i), bound, callback)) return false;
			}
		}
	}
	return true;
}

RTreeTemplate
typename RTreeScope::Node *RTreeScope::splitLeaf(RTreeScope::Node *node) {
	LeafNode split1, *split2;
	AesalonPoolAlloc(LeafNode, split2, LeafNode());
	
	/* Find maximum separation. */
	int highestStart[Dimensions];
	int lowestEnd[Dimensions];
	Key lowest[Dimensions];
	Key highest[Dimensions];
	
	for(int i = 0; i < Dimensions; i ++) {
		highestStart[i] = 0;
		lowestEnd[i] = 0;
		lowest[i] = node->asLeaf()->bound(0).start(i);
		highest[i] = node->asLeaf()->bound(0).end(i);
		
		for(int j = 1; j < Maximum; j ++) {
			const Key &start = node->asLeaf()->bound(j).start(i);
			const Key &end = node->asLeaf()->bound(j).end(i);
			if(start > node->asLeaf()->bound(j).start(highestStart[i])) highestStart[i] = j;
			if(end < node->asLeaf()->bound(j).end(lowestEnd[i])) lowestEnd[i] = j;
			if(start < lowest[i]) lowest[i] = start;
			if(end > highest[i]) highest[i] = end;
		}
	}
	
	FloatKey normalized[Dimensions];
	int largest = 0;
	for(int i = 0; i < Dimensions; i ++) {
		if(lowest[i] == highest[i]) {
			normalized[i] = 0;
		}
		else {
			normalized[i] = FloatKey(lowestEnd[i] - highestStart[i]) / FloatKey(highest[i]-lowest[i]);
		}
		
		if(normalized[i] > normalized[largest]) largest = i;
	}
	
	if(highestStart[largest] == lowestEnd[largest]) {
		split1.addBranch(node->asLeaf()->bound(0), node->asLeaf()->value(0));
		split1.bound() = node->asLeaf()->bound(0);
		split2->addBranch(node->asLeaf()->bound(1), node->asLeaf()->value(1));
		split2->bound() = node->asLeaf()->bound(1);
		node->asLeaf()->removeBranch(0);
		node->asLeaf()->removeBranch(0);
	}
	else {
		split1.addBranch(node->asLeaf()->bound(highestStart[largest]), node->asLeaf()->value(highestStart[largest]));
		split2->addBranch(node->asLeaf()->bound(lowestEnd[largest]), node->asLeaf()->value(lowestEnd[largest]));
		
		split1.bound() = node->asLeaf()->bound(highestStart[largest]);
		split2->bound() = node->asLeaf()->bound(lowestEnd[largest]);
		
		if(highestStart[largest] > lowestEnd[largest]) {
			node->asLeaf()->removeBranch(highestStart[largest]);
			node->asLeaf()->removeBranch(lowestEnd[largest]);
		}
		else {
			node->asLeaf()->removeBranch(lowestEnd[largest]);
			node->asLeaf()->removeBranch(highestStart[largest]);
		}
	}
	
	while(node->branches() > 0) {
		if(split1.branches() + node->branches() == Minimum) {
			for(int i = 0; i < node->branches(); i ++) {
				split1.addBranch(node->asLeaf()->bound(i), node->asLeaf()->value(i));
				split1.bound().cover(node->asLeaf()->bound(i));
			}

			break;
		}
		else if(split2->branches() + node->branches() == Minimum) {
			for(int i = 0; i < node->branches(); i ++) {
				split2->addBranch(node->asLeaf()->bound(i), node->asLeaf()->value(i));
				split2->bound().cover(node->asLeaf()->bound(i));
			}

			break;
		}
		
		Key cover1 = split1.bound().toCover(node->asLeaf()->bound(0));
		Key cover2 = split2->bound().toCover(node->asLeaf()->bound(0));
		Message(Debug, "Adding value " << node->asLeaf()->value(0));

		if(cover1 < cover2) {
			split1.addBranch(node->asLeaf()->bound(0), node->asLeaf()->value(0));
			split1.bound().cover(node->asLeaf()->bound(0));
		}
		else if(cover1 > cover2) {
			split2->addBranch(node->asLeaf()->bound(0), node->asLeaf()->value(0));
			split2->bound().cover(node->asLeaf()->bound(0));
		}
		else {
			if(split1.bound().volume() < split2->bound().volume()) {
				split1.addBranch(node->asLeaf()->bound(0), node->asLeaf()->value(0));
				split1.bound().cover(node->asLeaf()->bound(0));
			}
			else {
				split2->addBranch(node->asLeaf()->bound(0), node->asLeaf()->value(0));
				split2->bound().cover(node->asLeaf()->bound(0));
			}
		}
		node->asLeaf()->removeBranch(0);
		
		node->branches();
	}

	split1.setParent(node->parent());
	split2->setParent(node->parent());
	*node = split1;
	
	return split2;
}

RTreeTemplate
typename RTreeScope::Node *RTreeScope::splitInternal(RTreeScope::Node *node) {
	//AesalonPoolAlloc(Node, s, Node(node->depth(), NULL));
	
	
	
	Message(Fatal, "Splitting NYI.");
	
	return node;
}

} // namespace Storage

#endif
