#include "FreeEvent.h"

void FreeEvent::apply_to(Snapshot *snapshot) {
    BiTreeNode *node = snapshot->get_head_node();
    if(!node) return;
    
    while(true) {
        if(node->get_left() && address < node->get_address()) node = node->get_left();
        else if(node->get_right() && address >= node->get_address()) node = node->get_right();
        else break;
    }
    Block *block = node->get_block(address);
    if(block) node->remove_block(block);
}
