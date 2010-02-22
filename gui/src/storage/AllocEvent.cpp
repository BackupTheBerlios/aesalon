#include "AllocEvent.h"

void AllocEvent::apply_to(Snapshot *snapshot) {
    qDebug("Asked to apply AllocEvent to snapshot #%li . . .", (long int)snapshot->get_snapshot_id());
    /* Create the head node if it doesn't exist . . . */
    if(snapshot->get_head_node() == NULL) {
        qDebug("Creating new head node . . .");
        snapshot->set_head_node(new BiTreeNode(snapshot->get_snapshot_id()));
    }
    
    BiTreeNode *node = snapshot->get_head_node();
    
    quint8 max_depth = snapshot->get_max_tree_depth();
    /* First, traverse the tree to find the right spot. Create any nodes required to get there,
        and mark the created nodes' parents as changed, of course. */
    
    qDebug("Starting at the head of the tree . . .");
    
    /*          1010101010101010101010101010101010101010101010101010101010101010
        mask:   1000000000000000000000000000000000000000000000000000000000000000
        mask:   0100000000000000000000000000000000000000000000000000000000000000
        mask:   0010000000000000000000000000000000000000000000000000000000000000
        mask:   0001000000000000000000000000000000000000000000000000000000000000
        mask:   0000100000000000000000000000000000000000000000000000000000000000
        ....
        0x01 << depth;
        */
    for(quint8 depth = 63; depth > 64 - max_depth; depth --) {
        quint64 mask = 0x01;
        mask <<= depth;
        /*if(!(MemoryAddress(address << depth) & 0x01)) {*/
        if((address & mask) == 0) {
            if(node->get_left() == NULL) {
                node = node->mark_changed(snapshot->get_snapshot_id());
                node->set_left(new BiTreeNode(snapshot->get_snapshot_id()));
            }
            node = node->get_left();
        }
        else {
            if(node->get_right() == NULL) {
                node = node->mark_changed(snapshot->get_snapshot_id());
                node->set_right(new BiTreeNode(snapshot->get_snapshot_id()));
            }
            node = node->get_right();
        }
    }
    
    qDebug("AllocEvent: adding block to node . . . current size is %i.", node->get_block_list_size());
    /* Well, we're at the correct node to add the block into (hopefully, anyhow) . . . */
    node = node->mark_changed(snapshot->get_snapshot_id());
    node->add_block(new Block(address, size));
    
    /* Now, set the snapshot's new head node . . . */
    if(snapshot->get_head_node()->get_snapshot_id() != snapshot->get_snapshot_id()) {
        qDebug("AllocEvent: setting new head node for snapshot %i . . .", (int)snapshot->get_snapshot_id());
        BiTreeNode *head_node = node;
        while(head_node->get_parent()) head_node = head_node->get_parent();
        qDebug("New head node address is %p", (const void *)head_node);
        snapshot->set_head_node(head_node);
    }
    /* And update the snapshot's timestamp. */
    snapshot->update_timestamp();
}
