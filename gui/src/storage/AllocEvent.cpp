#include <cmath>

#include "AllocEvent.h"
#include "StorageFactory.h"

/* NOTE: this should be redefined somewhere, perhaps . . . assumes 64-bit address space . . . */
const MemoryAddress ADDRESS_MAX = 0xffffffffffffffff;

void AllocEvent::apply_to(Snapshot *snapshot) {
    qDebug("Asked to apply AllocEvent to snapshot #%li . . .", (long int)snapshot->get_snapshot_id());
    /* If the snapshot block tree head is NULL, then don't do anything special . . . */
    if(snapshot->get_head_node() == NULL) {
        snapshot->set_head_node(StorageFactory::new_node(snapshot->get_snapshot_id(), (ADDRESS_MAX/2)+1));
        /* Now just chain onto the other code . . . */
    }
    
    BiTreeNode *node = snapshot->get_head_node();
    
    quint8 max_depth = snapshot->get_max_tree_depth();
    qDebug("MemoryAddress is %p", address);
    for(quint8 depth = 0; depth < max_depth; depth ++) {
        bool bit = MemoryAddress(address << depth) & 0x01;
        qDebug("%dth bit is %s", depth, bit?"true":"false");
        if(bit) {
            if(node->get_right() == NULL) {
                node = node->mark_changed(snapshot->get_snapshot_id());
                node->set_right(StorageFactory::new_node(snapshot->get_snapshot_id()));
            }
        }
    }
}
