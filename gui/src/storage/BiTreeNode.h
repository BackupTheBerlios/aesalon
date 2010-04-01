/** Aesalon, a project to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009-2010 strange <kawk256@gmail.com>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    @file storage/BiTreeNode.h
*/

#ifndef AESALON_GUI_STORAGE_BI_TREE_NODE_H
#define AESALON_GUI_STORAGE_BI_TREE_NODE_H

#include <QList>

#include "MemoryTypes.h"
#include "SnapshotID.h"
#include "Block.h"

class BiTreeNode {
private:
    bool end;
    SnapshotID snapshot_id;
    
    /** The left node in the tree, e.g. the node that stores the blocks with
        lower addresses than @a address */
    BiTreeNode *left;
    /** The right node in the tree, e.g. the node that stores the blocks with
        higher addresses than @a address */
    BiTreeNode *right;
    
    QList<Block *> block_list;
public:
    BiTreeNode(SnapshotID snapshot_id) : end(true), snapshot_id(snapshot_id), left(NULL), right(NULL) {}
    virtual ~BiTreeNode() {}
    
    SnapshotID get_snapshot_id() const { return snapshot_id; }
    
    BiTreeNode *get_left() const { return left; }
    void set_left(BiTreeNode *new_left) { left = new_left; }
    BiTreeNode *get_right() const { return right; }
    void set_right(BiTreeNode *new_right) { right = new_right; }
    
    void add_block(Block *new_block) { block_list.append(new_block); }
    void remove_block(Block *block) { block_list.removeOne(block); }
    Block *get_block(MemoryAddress address) const;
    Block *get_block_for(MemoryAddress address) const;
    QList<Block *> &get_block_list() { return block_list; }
    int get_block_list_size() const { return block_list.size(); }
    
    bool is_end() const { return end; }
    void set_end(bool new_end) { end = new_end; }
    
    BiTreeNode *mark_changed(SnapshotID by_snapshot);
};

#endif
