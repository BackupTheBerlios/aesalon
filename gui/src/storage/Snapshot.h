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
    
    @file storage/Snapshot.h
*/

#ifndef AESALON_GUI_STORAGE_SNAPSHOT_H
#define AESALON_GUI_STORAGE_SNAPSHOT_H

#include "SnapshotID.h"
#include "BiTreeNode.h"
#include "EventList.h"
#include "Timestamp.h"

class Snapshot {
private:
    SnapshotID snapshot_id;
    BiTreeNode *head_node;
    EventList event_list;
    quint8 max_tree_depth;
    quint64 block_count;
    Timestamp timestamp;
public:
    Snapshot(SnapshotID snapshot_id);
    virtual ~Snapshot();
    
    SnapshotID get_snapshot_id() const { return snapshot_id; }
    
    BiTreeNode *get_head_node() const { return head_node; }
    void set_head_node(BiTreeNode *new_head) { head_node = new_head; }
    
    void add_event(Event *event) { event_list.add_event(event); }
    EventList *get_event_list() { return &event_list; }
    
    quint8 get_max_tree_depth() { return max_tree_depth; }
    
    quint64 get_block_count() const { return block_count; }
    void inc_block_count() { block_count ++; }
    void dec_block_count() { block_count --; }
    void set_block_count(quint64 new_block_count) { block_count = new_block_count; }
    Block *get_block(MemoryAddress address);
    
    const Timestamp &get_timestamp() const { return timestamp; }
    void update_timestamp(const Timestamp &new_timestamp) { timestamp = new_timestamp; }
    
    void free_tree();
};

#endif
