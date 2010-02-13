#include "SnapshotList.h"

SnapshotList::SnapshotList() {

}

SnapshotList::~SnapshotList() {

}

Snapshot *SnapshotList::append_snapshot() {
    Snapshot *s = new Snapshot(++last_id);
    internal_list.append(s);
    return s;
}
