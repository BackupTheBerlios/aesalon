#ifndef AESALON_GUI_STORAGE_SNAPSHOT_LIST_H
#define AESALON_GUI_STORAGE_SNAPSHOT_LIST_H

#include <QList>

#include "SnapshotID.h"
#include "Snapshot.h"
#include "Timestamp.h"

class SnapshotList {
private:
    SnapshotID last_id;
    SnapshotID last_temporary_id;
    QList<Snapshot *> internal_list;
public:
    SnapshotList();
    virtual ~SnapshotList();
    
    SnapshotID get_last_id() const { return last_id; }
    
    Snapshot *append_snapshot();
    Snapshot *get_snapshot(SnapshotID id) const { return internal_list[id-1]; }
    Snapshot *get_snapshot_for(const Timestamp &timestamp);
    int get_snapshot_count() const { return internal_list.size(); }
};

#endif
