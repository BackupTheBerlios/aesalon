#include "SnapshotList.h"
#include "Event.h"

SnapshotList::SnapshotList() : last_id(0), last_temporary_id(0) {

}

SnapshotList::~SnapshotList() {

}

Snapshot *SnapshotList::append_snapshot() {
    Snapshot *s = new Snapshot(++last_id);
    if(internal_list.size()) {
        s->set_head_node(internal_list.back()->get_head_node());
        s->set_block_count(internal_list.back()->get_block_count());
    }
    internal_list.append(s);
    return s;
}

Snapshot *SnapshotList::get_snapshot_for(const Timestamp &timestamp) {
    Snapshot *s = new Snapshot(alloc_temporary_id());
    int i;
    Snapshot *list_shot = get_closest_snapshot(timestamp);
    if(list_shot == NULL) {
        qDebug("Couldn't find snapshot for requested time: %s", timestamp.to_string().toStdString().c_str());
        return s;
    }
    s->set_head_node(list_shot->get_head_node());
    s->set_block_count(list_shot->get_block_count());
    s->update_timestamp(list_shot->get_timestamp());
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

Snapshot* SnapshotList::get_closest_snapshot(const Timestamp &timestamp) {
    Snapshot *list_shot = NULL;
    /* NOTE: speed this up via a binary-style search through the list: try size/2, size/2+size/4 or size/2-size/4 . . . */
    for(int i = 0; i < internal_list.size(); i ++) {
        if(timestamp >= internal_list[i]->get_timestamp())
            list_shot = internal_list[i];
    }
    return list_shot;
}

bool SnapshotList::move_snapshot_to_next_event(Snapshot *temporary_snapshot) {
    Snapshot *list_shot = get_closest_snapshot(temporary_snapshot->get_timestamp());
    QList<Event *> event_list = list_shot->get_event_list()->get_event_list();
    bool applied = false;
    do {
        for(int i = 0; i < event_list.size(); i ++) {
            Event *event = event_list[i];
            if(event->get_timestamp() >= temporary_snapshot->get_timestamp() && 
                !temporary_snapshot->get_event_list()->get_event_list().contains(event)) {
                
                event->apply_to(temporary_snapshot);
                temporary_snapshot->add_event(event);
                applied = true;
                break;
            }
        }
        if(list_shot->get_snapshot_id()+1 > internal_list.back()->get_snapshot_id()) {
            break;
        }
        list_shot = get_snapshot(list_shot->get_snapshot_id()+1);
        event_list = list_shot->get_event_list()->get_event_list();
    } while(!applied);
    return applied;
}
