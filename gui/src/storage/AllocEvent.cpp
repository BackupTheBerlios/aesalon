#include "AllocEvent.h"

void AllocEvent::apply_to(Snapshot *snapshot) {
    qDebug("Asked to apply AllocEvent to snapshot #%li . . .", (long int)snapshot->get_snapshot_id());
    /* Create the head node if it doesn't exist . . . */
    if(snapshot->get_head_node() == NULL) {
        snapshot->set_head_node(new BiTreeNode(snapshot->get_snapshot_id()));
    }
    
    snapshot->set_head_node(snapshot->get_head_node()->mark_changed(snapshot->get_snapshot_id()));
    
    BiTreeNode *node = snapshot->get_head_node();
    
    quint8 max_depth = snapshot->get_max_tree_depth();
    /* First, traverse the tree to find the right spot. Create any nodes required to get there. */
    
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
                node->set_left(new BiTreeNode(snapshot->get_snapshot_id()));
            }
            node = node->get_left();
        }
        else {
            if(node->get_right() == NULL) {
                node->set_right(new BiTreeNode(snapshot->get_snapshot_id()));
            }
            node = node->get_right();
        }
    }
    
    /* Well, we're at the correct node to add the block into (hopefully, anyhow) . . . so mark it as changed. */
    BiTreeNode *old_node = node;
    node = node->mark_changed(snapshot->get_snapshot_id());
    if(last_node->get_left() == old_node) last_node->set_left(node);
    else if(last_node->get_right() == old_node) last_node->set_right(node);
    /* Then add the block to the new, "changed" node. */
    node->add_block(new Block(address, size));
}
