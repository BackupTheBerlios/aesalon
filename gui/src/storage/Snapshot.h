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
    
    const Timestamp &get_timestamp() const { return timestamp; }
    void update_timestamp(const Timestamp &new_timestamp) { timestamp = new_timestamp; }
    
    void free_tree();
};

#endif
