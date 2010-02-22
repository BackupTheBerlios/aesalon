#include "SnapshotList.h"

SnapshotList::SnapshotList() : last_id(0) {

}

SnapshotList::~SnapshotList() {

}

Snapshot *SnapshotList::append_snapshot() {
    Snapshot *s = new Snapshot(++last_id);
    internal_list.append(s);
    return s;
}

Snapshot *SnapshotList::get_snapshot_for(const Timestamp &timestamp) const {
    Snapshot *s = new Snapshot(-1);
    /* NOTE: speed this up via a binary-style search through the list: try size/2, size/2+size/4 or size/2-size/4 . . . */
    int i;
    for(i = 0; i < internal_list.size(); i ++) {
        if(internal_list[i]->get_timestamp() >= timestamp) break;
    }
    if(i == internal_list.size()) {
        qDebug("Couldn't find snapshot for requested time: %s", timestamp.to_string().toStdString().c_str());
        return NULL;
    }
    s->set_head_node(internal_list[i]->get_head_node());
    return s;
}
