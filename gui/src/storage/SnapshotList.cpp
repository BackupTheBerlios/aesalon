#include "SnapshotList.h"

SnapshotList::SnapshotList() {

}

SnapshotList::~SnapshotList() {

}

Snapshot *SnapshotList::get_snapshot_by_id(SnapshotID snapshot_id) const {
    foreach(Snapshot *snapshot, internal_list) {
        if(snapshot->get_snapshot_id() == snapshot_id) return snapshot;
    }
    return NULL;
}
