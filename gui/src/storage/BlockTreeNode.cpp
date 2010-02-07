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
