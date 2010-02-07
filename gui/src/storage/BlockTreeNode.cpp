#include "BlockTreeNode.h"

bool BlockTreeNode::operator<(const BlockTreeNode &other) const {
    return address < other.address;
}

bool BlockTreeNode::operator>(const BlockTreeNode &other) const {
    return address > other.address;
}

bool BlockTreeNode::operator==(const BlockTreeNode &other) const {
    return address == other.address;
}

BlockTreeNode *BlockTreeNode::find_block_by_address(MemoryAddress address) const {
    if(address == get_address()) return this;
    if(address < get_left()->get_address())
    return NULL;
}