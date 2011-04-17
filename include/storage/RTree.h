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
private:
	typedef RTreePrivate::Node<DataType, BoundType, MaximumFactor> NodeType;
	typedef RTreePrivate::InternalNode<DataType, BoundType, MaximumFactor> InternalNodeType;
	typedef RTreePrivate::LeafNode<DataType, BoundType, MaximumFactor> LeafNodeType;
	
	NodeType *m_root;
public:
	RTree() {
		m_root = NULL;
	}
	
	~RTree() {
		
	}
	
	void insert(BoundType bound, DataType data) {
		if(m_root == NULL) {
			LeafNodeType *leafRoot;
			AesalonPoolAlloc(LeafNodeType, leafRoot, LeafNodeType());
			leafRoot->addBranch(bound, data);
			m_root = leafRoot;
			return;
		}
		
	}
};

} // namespace Storage

#endif
