#ifndef AESALON_GUI_STORAGE_SNAPSHOT_H
#define AESALON_GUI_STORAGE_SNAPSHOT_H

#include "SnapshotID.h"
#include "BlockTreeNode.h"

class Snapshot {
private:
    BlockTreeNode *head_node;
    SnapshotID snapshot_id;
public:
    Snapshot(SnapshotID snapshot_id);
    virtual ~Snapshot();
    
    SnapshotID get_snapshot_id() const { return snapshot_id; }
    
    BlockTreeNode *get_head_node() const { return head_node; }
    void set_head_node(BlockTreeNode *new_head) { head_node = new_head; }
};

#endif
