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
    
    BlockTreeNode *get_head_node() const;
};

#endif
