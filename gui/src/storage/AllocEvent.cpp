#include "AllocEvent.h"
#include "StorageFactory.h"

/* NOTE: this should be redefined somewhere, perhaps . . . assumes 64-bit address space . . . */
const MemoryAddress ADDRESS_MAX = 0xffffffffffffffff;

void AllocEvent::apply_to(Snapshot *snapshot) {
    qDebug("Asked to apply AllocEvent to snapshot #%i . . .", snapshot->get_snapshot_id());
    /* If the snapshot block tree head is NULL, then don't do anything special . . . */
    if(snapshot->get_head_node() == NULL) {
        snapshot->set_head_node(StorageFactory::new_node(snapshot->get_snapshot_id(), (ADDRESS_MAX/2)+1));
        /* Now just chain onto the other code . . . */
    }
    
    BiTreeNode *node = snapshot->get_head_node();
    /* Find the correct node to split . . . */
    while(true) {
        if(node->get_left() && get_address() < node->get_address())
            node = node->get_left();
        else if(node->get_right() && get_address() >= node->get_address())
            node = node->get_right();
        else break;
    }
    
    BiTreeNode *n = NULL;
    
    Block *block = StorageFactory::new_block(address, size);
    
    /* Now add the new block into the correct spot . . . */
    MemoryAddress diff = 0;
    if(node->get_parent()) diff = node->get_parent()->get_address() - node->get_address();
    else diff = node->get_address();
    diff /= 2;
    /* First, handle the special cases of a NULL left/right */
    
    if(node->get_left() == NULL && address < node->get_address()) {
        n = StorageFactory::new_node(snapshot->get_snapshot_id(), address);
        node->mark_changed(snapshot->get_snapshot_id());
    }
    
}
