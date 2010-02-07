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
    BlockTreeNode *parent;
    
    MemoryAddress address;
    MemorySize size;
    
    SnapshotID snapshot_id;
public:
    BlockTreeNode(SnapshotID snapshot_id) : left(NULL), right(NULL), parent(NULL), address(0), size(0), snapshot_id(snapshot_id) {}
    virtual ~BlockTreeNode() {}
    
    BlockTreeNode *get_left() const { return left; }
    void set_left(BlockTreeNode *new_left) { left = new_left; if(left) left->set_parent(this); }
    BlockTreeNode *get_right() const { return right; }
    void set_right(BlockTreeNode *new_right) { right = new_right; if(right) right->set_parent(this); }
    BlockTreeNode *get_parent() const { return parent; }
    void set_parent(BlockTreeNode *new_parent) { parent = new_parent; }
    
    /* NOTE: these operators operate solely on address, not on size . . . */
    bool operator<(const BlockTreeNode &other) const;
    bool operator>(const BlockTreeNode &other) const;
    bool operator==(const BlockTreeNode &other) const;
    
    bool stores_data() const { return size != 0; }
    
    MemoryAddress get_address() const { return address; }
    void set_address(MemoryAddress new_address) { address = new_address; }
    MemorySize get_size() const { return size; }
    void set_size(MemorySize new_size) { size = new_size; }
    
    BlockTreeNode *find_block_by_address(MemoryAddress address) const;
};

#endif
