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
		Bound() {}
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
		}
		
		void cover(const Bound &other) {
			for(int i = 0; i < Dimensions; i ++) {
				m_bounds[i][0] = std::min(m_bounds[i][0], other.m_bounds[i][0]);
				m_bounds[i][1] = std::max(m_bounds[i][1], other.m_bounds[i][1]);
			}
		}
	};
	
	class Visitor {
	public:
		virtual ~Visitor() {}
		
		virtual void visit(const Bound &bound, Value &value) = 0;
	};
protected:
	class Node {
	private:
		Bound m_bound;
		int m_depth;
		int m_childCount;
		Node *m_parent, *m_next;
		union {
			Value m_value;
			Node *m_child;
		};
	public:
		Node(const Bound &bound, Value value, Node *parent)
			: m_bound(bound), m_depth(0), m_childCount(0), m_parent(parent), m_next(NULL), m_value(value) { }
		Node(int depth, Node *parent)
			: m_depth(depth), m_childCount(0), m_parent(parent), m_next(NULL) {}
		
		Bound &bound() { return m_bound; }
		const Bound &bound() const { return m_bound; }
		
		int depth() const { return m_depth; }
		bool isLeaf() const { return m_depth == 0; }
		
		bool isFull() const { return m_childCount == Maximum; }
		int childCount() const { return m_childCount; }
		void incChildren() { m_childCount ++; }
		void setChildCount(int childcount) { m_childCount = childcount; }
		
		Node *parent() const { return m_parent; }
		void setParent(Node *parent) { m_parent = parent; }
		
		Node *next() const { return m_next; }
		void setNext(Node *next) { m_next = next; }
		
		Value &value() { return m_value; }
		const Value &value() const { return m_value; }
		void setValue(Value value) { m_value = value; }
		Node *child() const { return m_child; }
		void setChild(Node *child) { m_child = child; }
	};
private:
	Node *m_root;
public:
	RTree();
	~RTree();
	
	void search(const Bound &bound, Visitor *visitor);
	void insert(const Bound &bound, const Value &value);
private:
	void search(Node *node, const Bound &bound, Visitor *visitor);
	Node *split(Node *node, Node *toInsert);
};

RTreeTemplate
RTreeScope::RTree() {
	m_root = NULL;
}

RTreeTemplate
RTreeScope::~RTree() {
	
}

RTreeTemplate
void RTreeScope::search(const RTreeScope::Bound &bound, RTreeScope::Visitor *visitor) {
	search(m_root, bound, visitor);
}

RTreeTemplate
void RTreeScope::insert(const RTreeScope::Bound &bound, const Value &value) {
	Node *toInsert;
	AesalonPoolAlloc(Node, toInsert, Node(bound, value, NULL));
	
	if(m_root == NULL) {
		AesalonPoolAlloc(Node, m_root, Node(0, NULL));
	}
	
	/* Choose leaf node . . . */
	Node *c = m_root;
	while(c->child() && !c->child()->isLeaf()) {
		c = c->child();
	}
	
	Node *cc = NULL;
	if(!c->isFull()) {
		Message(Debug, "First insertion case: c->child() is " << c->child());
		toInsert->setNext(c->child());
		toInsert->setParent(c);
		c->setChild(toInsert);
		c->incChildren();
	}
	else {
		cc = split(c, toInsert);
	}
	
	while(cc != NULL && c != m_root) {
		Node *parent = c->parent();
		
		if(!parent->isFull()) {
			cc->setNext(parent->child());
			cc->setParent(parent);
			parent->setChild(cc);
			parent->incChildren();
		}
		else {
			cc = split(parent, cc);
		}
		
		c = parent;
	}
	
	if(cc != NULL) {
		Node *newRoot;
		AesalonPoolAlloc(Node, newRoot, Node(m_root->depth()+1, NULL));
		c->setParent(newRoot);
		cc->setParent(newRoot);
		c->setNext(cc);
		newRoot->setChildCount(2);
		newRoot->setChild(c);
	}
}

RTreeTemplate
void RTreeScope::search(RTreeScope::Node *node, const RTreeScope::Bound &bound, RTreeScope::Visitor *visitor) {
	if(node == NULL) return;
	
	Message(Debug, "**** Beginning search.");
	
	Node *n = node;
	while(n != NULL) {
		Message(Debug, "Searching node . . .");
		if(bound.overlaps(n->bound())) {
			Message(Debug, "\tBound overlaps!");
			if(node->isLeaf()) {
				visitor->visit(n->bound(), n->value());
			}
			else {
				search(n, bound, visitor);
			}
		}
		n = n->next();
	}
}

RTreeTemplate
typename RTreeScope::Node *RTreeScope::split(RTreeScope::Node *node, RTreeScope::Node *toInsert) {
	Node *s;
	AesalonPoolAlloc(Node, s, Node(node->depth(), NULL));
	
	
	Message(Fatal, "Splitting NYI.");
	
	toInsert = toInsert;
	
	return s;
}

} // namespace Storage

#endif
