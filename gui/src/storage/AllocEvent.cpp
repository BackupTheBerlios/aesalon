#include "AllocEvent.h"
#include "StorageFactory.h"

/* NOTE: this should be redefined somewhere, perhaps . . . assumes 64-bit address space . . . */
const MemoryAddress ADDRESS_HALFWAY = 0x8000000000000000;

/* NOTE: this function will probably not work . . . */
void AllocEvent::apply_to(Snapshot *snapshot) {
    if(snapshot->get_head_node() == NULL) {
        snapshot->set_head_node(StorageFactory::new_node(snapshot->get_snapshot_id(), ADDRESS_HALFWAY));
        snapshot->get_head_node()->add_block(new Block(address, size));
        return;
    }
    BiTreeNode *node = snapshot->get_head_node();
    while(!node->is_end()) {
        if(node->get_left() && address < node->get_left()->get_address()) node = node->get_left();
        else if(node->get_right() && address > node->get_right()->get_address()) node = node->get_right();
        else break;
    }
    if(address < node->get_address()) {
        node->set_left(StorageFactory::new_node(snapshot->get_snapshot_id(), node->get_address()/2));
        node->mark_changed(snapshot->get_snapshot_id());
        node->set_end(false);
        node->get_left()->add_block(new Block(address, size));
    }
    else if(address >= node->get_address()) {
        
    }
}
