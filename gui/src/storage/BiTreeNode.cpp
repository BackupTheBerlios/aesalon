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
    BiTreeNode *new_node = new BiTreeNode(by_snapshot);
    new_node->left = left;
    if(left) left->parent = new_node;
    new_node->right = right;
    if(right) right->parent = new_node;
    new_node->end = end;
    new_node->block_list = block_list;
    if(parent) {
        new_node->parent = parent->mark_changed(by_snapshot);
        if(parent->get_left() == this) new_node->parent->left = new_node;
        else if(parent->get_right() == this) new_node->parent->right = new_node;
    }
    return new_node;
}
