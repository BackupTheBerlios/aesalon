#include "FreeEvent.h"

void FreeEvent::apply_to(Snapshot *snapshot) {
    qDebug("Asked to apply FreeEvent to snapshot #%li . . .", (long int)snapshot->get_snapshot_id());
    BiTreeNode *node = snapshot->get_head_node();
    if(!node) return;
    
    quint8 max_depth = snapshot->get_max_tree_depth();
    /* First, traverse the tree to find the right spot. Do not create any
        nodes required to get there -- this is a removal, not an addition! */
    for(quint8 depth = 0; depth < max_depth; depth ++) {
        bool bit = MemoryAddress(address << depth) & 0x01;
        if(!bit) {
            if(node->get_left() != NULL) node = node->get_left();
            else return;
        }
        else {
            if(node->get_right() != NULL) node = node->get_right();
            else return;
        }
    }
    Block *block = node->get_block(address);
    if(block) node->remove_block(block);
}
