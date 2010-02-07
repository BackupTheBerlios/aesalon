#ifndef AESALON_GUI_STORAGE_NODE_H
#define AESALON_GUI_STORAGE_NODE_H

#include <cstddef>
#include <cctype>

#include "MemoryTypes.h"
#include "SnapshotID.h"

class BlockTreeNode {
private:
    BlockTreeNode *left;
    BlockTreeNode *right;
    
    MemoryAddress address;
    MemoryAddress size;
    
    SnapshotID snapshot_id;
public:
    BlockTreeNode(SnapshotID snapshot_id) : left(NULL), right(NULL), address(0), size(0), snapshot_id(snapshot_id) {}
    virtual ~BlockTreeNode();
    
    BlockTreeNode *get_left() const { return left; }
    void set_left(BlockTreeNode *new_left) { left = new_left; }
    BlockTreeNode *get_right() const { return right; }
    void set_right(BlockTreeNode *new_right) { right = new_right; }
    
    /* NOTE: these operators operate solely on address, not on size . . . */
    bool operator<(const BlockTreeNode &other) const;
    bool operator>(const BlockTreeNode &other) const;
    bool operator==(const BlockTreeNode &other) const;
    
    bool stores_data() const { return size != 0; }
    
    MemoryAddress get_address() const { return address; }
    MemoryAddress get_size() const { return size; }
};

#endif
