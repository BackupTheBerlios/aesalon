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
    
    @file storage/FreeEvent.cpp
*/

#include "FreeEvent.h"
#include "EventVisitor.h"

void FreeEvent::apply_to(Snapshot *snapshot) {
    if(address == 0) {
        /* According to the man page for free(3): "If ptr is NULL, no operation is performed"->
            As such . . . */
        return;
    }
    if(!snapshot->get_head_node()) {
        qCritical("Asked to remove block from non-existent tree . . .");
        return;
    }
    
    snapshot->set_head_node(snapshot->get_head_node()->mark_changed(snapshot->get_snapshot_id()));
    BiTreeNode *node = snapshot->get_head_node();
    
    quint8 max_depth = snapshot->get_max_tree_depth();
    /* First, traverse the tree to find the right spot. Do not create any
        nodes required to get there -- this is a removal, not an addition! */
    BiTreeNode *last_node = snapshot->get_head_node();
    for(quint8 depth = 63; depth > 64 - max_depth; depth --) {
        quint64 mask = 0x01;
        mask <<= depth;
        
        /* Mark it as we're traversing the tree downwards (save the trouble later) . . . */
        
        BiTreeNode *old_node = node;
        node = node->mark_changed(snapshot->get_snapshot_id());
        if(last_node->get_left() == old_node) last_node->set_left(node);
        else if(last_node->get_right() == old_node) last_node->set_right(node);
        last_node = node;
        if((address & mask) == 0) {
            if(node->get_left() == NULL) {
                qCritical("Couldn't traverse tree to remove block (address of block was %llx)", address);
                return;
            }
            node = node->get_left();
        }
        else {
            if(node->get_right() == NULL) {
                qCritical("Couldn't traverse tree to remove block (address of block was %llx)", address);
                return;
            }
            node = node->get_right();
        }
    }
    
    Block *block = node->get_block(address);
    if(block) {
        BiTreeNode *old_node = node;
        node = node->mark_changed(snapshot->get_snapshot_id());
        if(last_node->get_left() == old_node) last_node->set_left(node);
        else if(last_node->get_right() == old_node) last_node->set_right(node);
        block->set_release_time(get_timestamp());
        block->set_release_scope(get_scope());
        
        node->remove_block(block);
        snapshot->dec_block_count();
    }
    else {
        qCritical("Asked to remove non-existent block (address of block was %llx)", address);
    }
    snapshot->update_timestamp(get_timestamp());
}

void FreeEvent::accept(EventVisitor &visitor) {
    visitor.visit(this);
}
