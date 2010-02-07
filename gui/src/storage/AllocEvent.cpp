#include "AllocEvent.h"
#include "StorageFactory.h"

/* NOTE: this function will probably not work . . . */
void AllocEvent::apply_to(Snapshot *snapshot) {
    BlockTreeNode *node, *n;
    if(snapshot->get_head_node() == NULL) {
        snapshot->set_head_node(StorageFactory::new_node(snapshot->get_snapshot_id()));
        node = StorageFactory::new_node(snapshot->get_snapshot_id());
        node->set_address(address);
        node->set_size(size);
        snapshot->get_head_node()->set_left(node);
        return;
    }
    node = snapshot->get_head_node();
    
    while(node) {
        if(node->get_left() && node->get_left()->get_address() > address) node = node->get_left();
        else if(node->get_right() && node->get_right()->get_address() < address) node = node->get_right();
        else break;
    }
    n = StorageFactory::new_node(snapshot->get_snapshot_id());
    n->set_address(address);
    n->set_size(size);
    node->set_left(n);
}
