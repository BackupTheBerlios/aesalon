#ifndef AESALON_GUI_STORAGE_SNAPSHOT_LIST_H
#define AESALON_GUI_STORAGE_SNAPSHOT_LIST_H

#include <QList>

#include "SnapshotID.h"
#include "Snapshot.h"
#include "StorageFactory.h"

class SnapshotList {
private:
    QList<Snapshot *> internal_list;
public:
    SnapshotList();
    virtual ~SnapshotList();
    
    Snapshot *append_snapshot() { Snapshot *s = StorageFactory::new_snapshot(); internal_list.append(s); return s; }
    Snapshot *get_snapshot(int index) const { return internal_list[index]; }
    int get_snapshot_count() const { return internal_list.size(); }
    Snapshot *get_snapshot_by_id(SnapshotID snapshot_id) const;
};

#endif
