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
    
    @file storage/BiTreeNode.cpp
*/

#include "BiTreeNode.h"
#include "Snapshot.h"

Block *BiTreeNode::get_block(MemoryAddress address) const {
    foreach(Block *block, block_list) {
        if(block->get_address() == address) return block;
    }
    return NULL;
}

Block *BiTreeNode::get_block_for(MemoryAddress address) const {
    foreach(Block *block, block_list) {
        if(block->get_address() <= address && address <= (block->get_address() + block->get_size())) return block;
    }
    qDebug("No suitable block found . . .");
    return NULL;
}


BiTreeNode *BiTreeNode::mark_changed(SnapshotID by_snapshot) {
    if(snapshot_id == by_snapshot) return this;
    BiTreeNode *new_node = new BiTreeNode(by_snapshot);
    new_node->left = left;
    new_node->right = right;
    new_node->end = end;
    new_node->block_list = block_list;
    return new_node;
}
