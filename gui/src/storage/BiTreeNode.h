#ifndef AESALON_GUI_STORAGE_BI_TREE_NODE_H
#define AESALON_GUI_STORAGE_BI_TREE_NODE_H

#include <QList>

#include "MemoryTypes.h"
#include "SnapshotID.h"
#include "Block.h"

class BiTreeNode {
private:
    bool end;
    MemoryAddress address;
    SnapshotID snapshot_id;
    
    BiTreeNode *left, *right, *parent;
    
    QList<Block *> block_list;
public:
    BiTreeNode(SnapshotID snapshot_id, MemoryAddress address) : snapshot_id(snapshot_id), address(address), end(true) {}
    virtual ~BiTreeNode() {}
    
    MemoryAddress get_address() const { return address; }
    BiTreeNode *get_left() const { return left; }
    void set_left(BiTreeNode *new_left) { left = new_left; if(left) new_left->set_parent(this); }
    BiTreeNode *get_right() const { return right; }
    void set_right(BiTreeNode *new_right) { right = new_right; if(right) right->set_parent(this); }
    BiTreeNode *get_parent() const { return parent; }
    void set_parent(BiTreeNode *new_parent) { parent = new_parent; }
    
    void add_block(Block *new_block) { block_list.append(new_block); }
    void remove_block(Block *block) { block_list.removeOne(block); }
    Block *get_block(MemoryAddress address) const;
    QList<Block *> &get_block_list() { return block_list; }
    int get_block_list_size() const { return block_list.size(); }
    
    bool is_end() const { return end; }
    void set_end(bool new_end) { end = new_end; }
    
    void mark_changed(SnapshotID by_snapshot);
};

#endif
