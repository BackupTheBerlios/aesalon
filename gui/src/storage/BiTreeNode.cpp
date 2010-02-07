#include "BiTreeNode.h"
#include "Snapshot.h"

Block *BiTreeNode::get_block(MemoryAddress address) const {
    foreach(Block *block, block_list) {
        if(block->get_address() == address) return block;
    }
    return NULL;
}

void BiTreeNode::mark_changed(SnapshotID by_snapshot) {
    
}
