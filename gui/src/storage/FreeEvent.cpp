#include "FreeEvent.h"

void FreeEvent::apply_to(Snapshot *snapshot) {
    qDebug("Asked to apply FreeEvent to snapshot #%li . . .", (long int)snapshot->get_snapshot_id());
    BiTreeNode *node = snapshot->get_head_node();
    if(!node) return;
    
    snapshot->set_head_node(snapshot->get_head_node()->mark_changed(snapshot->get_snapshot_id()));
    
    quint8 max_depth = snapshot->get_max_tree_depth();
    /* First, traverse the tree to find the right spot. Do not create any
        nodes required to get there -- this is a removal, not an addition! */
    BiTreeNode *last_node = snapshot->get_head_node();
    for(quint8 depth = 63; depth > 64 - max_depth; depth --) {
        quint64 mask = 0x01;
        mask <<= depth;
        
        /* Mark it as we're traversing the tree downwards (save the trouble later) . . . */
        
        BiTreeNode *old_node = node;
        node = node->mark_changed(snapshot->get_snapshot_id());
        if(last_node->get_left() == old_node) last_node->set_left(node);
        else if(last_node->get_right() == old_node) last_node->set_right(node);
        last_node = node;
        if((address & mask) == 0) {
            if(node->get_left() == NULL) {
                qCritical("Couldn't traverse tree to remove block . . .");
                return;
            }
            node = node->get_left();
        }
        else {
            if(node->get_right() == NULL) {
                qCritical("Couldn't traverse tree to remove block . . .");
                return;
            }
            node = node->get_right();
        }
    }
    
    Block *block = node->get_block(address);
    if(!block) {
        qCritical("Asked to remove non-existent block . . .");
    }
    if(block) {
        BiTreeNode *old_node = node;
        node = node->mark_changed(snapshot->get_snapshot_id());
        if(last_node->get_left() == old_node) last_node->set_left(node);
        else if(last_node->get_right() == old_node) last_node->set_right(node);
        node->remove_block(block);
    }
}
