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
		}
		
		void cover(const Bound &other) {
			for(int i = 0; i < Dimensions; i ++) {
				m_bounds[i][0] = std::min(m_bounds[i][0], other.m_bounds[i][0]);
				m_bounds[i][1] = std::max(m_bounds[i][1], other.m_bounds[i][1]);
			}
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
	class Node {
	private:
		Bound m_bound;
		int m_depth;
		int m_branches;
		Node *m_parent;
		
		union Branch {
			Value value;
			Node *child;
		};
		Branch m_branch[Maximum];
	public:
		Node(int depth) : m_depth(depth) {}
		
		Bound &bound() { return m_bound; }
		const Bound &bound() const { return m_bound; }
		
		int depth() const { return m_depth; }
		bool isLeaf() const { return m_depth == 0; }
		
		bool isFull() const { return m_branches == Maximum; }
		int branches() const { return m_branches; }
		
		void addBranch(const Bound &bound, Value value) {
			m_branch[m_branches].item.bound = bound;
			m_branch[m_branches++].item.value= value;
		}
		void addBranch(Node *child) {
		
		}
		
		Node *parent() const { return m_parent; }
		void setParent(Node *parent) { m_parent = parent; }
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
void RTreeScope::search(const RTreeScope::Bound &bound, RTreeScope::Callback *callback) {
	search(m_root, bound, callback);
}

RTreeTemplate
void RTreeScope::insert(const RTreeScope::Bound &bound, const Value &value) {
	Node *toInsert;
	AesalonPoolAlloc(Node, toInsert, Node(bound, value, NULL));
	
	
}

RTreeTemplate
bool RTreeScope::search(RTreeScope::Node *node, const RTreeScope::Bound &bound, RTreeScope::Callback *callback) {
	if(node == NULL) return;
	
	Message(Debug, "**** Beginning search.");
	
	Node *n = node;
	while(n != NULL) {
		Message(Debug, "Searching node . . .");
		if(bound.overlaps(n->bound())) {
			Message(Debug, "\tBound overlaps!");
			if(node->isLeaf()) {
				if(!callback->handle(n->bound(), n->value())) return false;
			}
			else {
				if(!search(n, bound, callback)) return false;
			}
		}
		n = n->next();
	}
	return true;
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
