#include "AllocEvent.h"
#include "StorageFactory.h"

/* NOTE: this should be redefined somewhere, perhaps . . . assumes 64-bit address space . . . */
const MemoryAddress ADDRESS_MAX = 0xffffffffffffffff;

void AllocEvent::apply_to(Snapshot *snapshot) {
    /* If the snapshot block tree head is NULL, then don't do anything special . . . */
    if(snapshot->get_head_node() == NULL) {
        snapshot->set_head_node(StorageFactory::new_node(snapshot->get_snapshot_id(), (ADDRESS_MAX/2)+1));
        snapshot->get_head_node()->set_end(false);
        BiTreeNode *node = snapshot->get_head_node();
        if(get_address() < node->get_address()) {
            node = StorageFactory::new_node(snapshot->get_snapshot_id(), node->get_address() / 2);
            snapshot->get_head_node()->set_left(node);
        }
        else if(get_address() > node->get_address()) {
            node = StorageFactory::new_node(snapshot->get_snapshot_id(), node->get_address() + (node->get_address() / 2));
            snapshot->get_head_node()->set_right(node);
        }
        else {
            qWarning("NOTE: attempting to add block on node boundary . . .");
            node = StorageFactory::new_node(-1, 0);
        }
        node->add_block(StorageFactory::new_block(address, size));
        return;
    }
    BiTreeNode *node = snapshot->get_head_node();
    /* Find the correct node to split . . . */
    while(true) {
        if(node->get_left() && get_address() < node->get_address())
            node = node->get_left();
        else if(node->get_right() && get_address() > node->get_address())
            node = node->get_right();
        else if(get_address() == node->get_address()) {
            /* Special case: print warning and exit for now . . . */
            qFatal("NOTE: attempted to add onto block storage node boundary!");
            return;
        }
        else break;
    }
    /* Now add the new block into the correct spot . . . */
    MemoryAddress diff = 0;
    if(node->get_parent()) diff = node->get_parent()->get_address() - node->get_address();
    else diff = node->get_address();
    diff /= 2;
    /* First, handle the special cases of a NULL left/right */
    if(node->get_right() == NULL && address > node->get_address()) {
        BiTreeNode *n = StorageFactory::new_node(snapshot->get_snapshot_id(), node->get_address() - diff);
        node->set_right(n);
        node->set_end(false);
        n->add_block(StorageFactory::new_block(get_address(), get_size()));
        return;
    }
    else if(node->get_left() == NULL && address < node->get_address()) {
        BiTreeNode *n = StorageFactory::new_node(snapshot->get_snapshot_id(), node->get_address() + diff);
        node->set_left(n);
        node->set_end(false);
        n->add_block(StorageFactory::new_block(get_address(), get_size()));
        return;
    }
    
}
