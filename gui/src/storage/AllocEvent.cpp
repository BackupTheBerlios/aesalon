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
    
    @file storage/AllocEvent.cpp
*/

#include "AllocEvent.h"
#include "EventVisitor.h"

void AllocEvent::apply_to(Snapshot *snapshot) {
    /* Create the head node if it doesn't exist . . . */
    if(snapshot->get_head_node() == NULL) {
        snapshot->set_head_node(new BiTreeNode(snapshot->get_snapshot_id()));
    }
    
    snapshot->set_head_node(snapshot->get_head_node()->mark_changed(snapshot->get_snapshot_id()));
    
    BiTreeNode *node = snapshot->get_head_node();
    
    quint8 max_depth = snapshot->get_max_tree_depth();
    /* First, traverse the tree to find the right spot. Create any nodes required to get there. */
    
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
                node->set_left(new BiTreeNode(snapshot->get_snapshot_id()));
            }
            node = node->get_left();
        }
        else {
            if(node->get_right() == NULL) {
                node->set_right(new BiTreeNode(snapshot->get_snapshot_id()));
            }
            node = node->get_right();
        }
    }
    
    /* Well, we're at the correct node to add the block into (hopefully, anyhow) . . . so mark it as changed. */
    BiTreeNode *old_node = node;
    node = node->mark_changed(snapshot->get_snapshot_id());
    if(last_node->get_left() == old_node) last_node->set_left(node);
    else if(last_node->get_right() == old_node) last_node->set_right(node);
    /* Then add the block to the new, "changed" node. */
    node->add_block(new Block(get_timestamp(), address, size));
    snapshot->inc_block_count();
    snapshot->update_timestamp(get_timestamp());
}

void AllocEvent::accept(EventVisitor &visitor) {
    visitor.visit(this);
}
