#include "FreeEvent.h"

void FreeEvent::apply_to(Snapshot *snapshot) {
    qDebug("Asked to apply FreeEvent to snapshot #%li . . .", (long int)snapshot->get_snapshot_id());
    BiTreeNode *node = snapshot->get_head_node();
    if(!node) return;
    
    quint8 max_depth = snapshot->get_max_tree_depth();
    /* First, traverse the tree to find the right spot. Do not create any
        nodes required to get there -- this is a removal, not an addition! */
    for(quint8 depth = 63; depth > 64 - max_depth; depth --) {
        quint64 mask = 0x01;
        mask <<= depth;
        if((address & mask) == 0) {
            if(node->get_left() != NULL) node = node->get_left();
            else return;
        }
        else {
            if(node->get_right() != NULL) node = node->get_right();
            else return;
        }
    }
    Block *block = node->get_block(address);
    if(block) {
        node = node->mark_changed(snapshot->get_snapshot_id());
        int blocks = node->get_block_list_size();
        node->remove_block(block);
        if(blocks == node->get_block_list_size()) {
            qDebug("Failed to remove block!");
        }
        else qDebug("Successfully removed block . . .");
        
        if(snapshot->get_head_node()->get_snapshot_id() != snapshot->get_snapshot_id()) {
            qDebug("FreeEvent: setting new head node for snapshot %i . . .", (int)snapshot->get_snapshot_id());
            BiTreeNode *head_node = node;
            while(head_node->get_parent()) head_node = head_node->get_parent();
            qDebug("New head node address is %p", (const void *)head_node);
            snapshot->set_head_node(head_node);
        }
    }
}
