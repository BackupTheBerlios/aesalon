#include "BiTreeNode.h"
#include "Snapshot.h"

Block *BiTreeNode::get_block(MemoryAddress address) const {
    foreach(Block *block, block_list) {
        if(block->get_address() == address) return block;
    }
    return NULL;
}

BiTreeNode *BiTreeNode::mark_changed(SnapshotID by_snapshot) {
    if(snapshot_id == by_snapshot) return this;
    BiTreeNode *new_node = new BiTreeNode(by_snapshot, address);
    new_node->left = left;
    new_node->right = right;
    new_node->end = end;
    new_node->block_list = block_list;
    new_node->parent = parent->mark_changed(by_snapshot);
    return new_node;
}
