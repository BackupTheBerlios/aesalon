/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/storage/RTree.cpp
*/

#include "storage/RTree.h"

namespace Storage {

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum>
RTree<Key, Value, Dimensions, Maximum, Minimum>::RTree() {
	m_root = NULL;
}

template<typename Key, typename Value, int Dimensions, int Maximum, int Minimum>
void RTree<Key, Value, Dimensions, Maximum, Minimum>::search(
	const RTree<Key, Value, Dimensions, Maximum, Minimum>::Bound &bound,
	RTree<Key, Value, Dimensions, Maximum, Minimum>::SearchProcessor *processor) {
	
	search(bound, m_root, processor);
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



} // namespace Storage
