#include "SnapshotList.h"
#include "Event.h"

SnapshotList::SnapshotList() : last_id(0), last_temporary_id(0) {

}

SnapshotList::~SnapshotList() {

}

Snapshot *SnapshotList::append_snapshot() {
    Snapshot *s = new Snapshot(++last_id);
    if(internal_list.size()) s->set_head_node(internal_list.back()->get_head_node());
    internal_list.append(s);
    return s;
}

Snapshot *SnapshotList::get_snapshot_for(const Timestamp &timestamp) {
    Snapshot *s = new Snapshot(--last_temporary_id);
    /* NOTE: speed this up via a binary-style search through the list: try size/2, size/2+size/4 or size/2-size/4 . . . */
    int i;
    Snapshot *list_shot = NULL;
    for(i = 0; i < internal_list.size(); i ++) {
        if(timestamp >= internal_list[i]->get_timestamp()) list_shot = internal_list[i];
    }
    if(list_shot == NULL) {
        qDebug("Couldn't find snapshot for requested time: %s", timestamp.to_string().toStdString().c_str());
        return s;
    }
    s->set_head_node(list_shot->get_head_node());
    /* Now apply the events to get the exact representation . . . */
    EventList *el = list_shot->get_event_list();
    QList<Event *> event_list = el->get_event_list();
    for(i = 0; i < event_list.size(); i ++) {
        Event *event = event_list[i];
        if(event->get_timestamp() > timestamp) break;
        event->apply_to(s);
        s->add_event(event);
    }
    return s;
}
