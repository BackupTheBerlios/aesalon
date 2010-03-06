#include <QList>
#include <QtAlgorithms>
#include <algorithm>

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
    QList<Snapshot *>::iterator begin = internal_list.begin(), end = internal_list.end(), middle;
    int n = end - begin;
    int half;

    /*foreach(Snapshot *snapshot, internal_list) {
        qDebug("Snapshot %lli, time is %s", snapshot->get_snapshot_id(), qPrintable(snapshot->get_timestamp().to_string()));
    }*/
    while (n > 0) {
        half = n / 2;
        middle = begin + half;
        if(timestamp > (*middle)->get_timestamp()) {
            begin = middle + 1;
            n -= half;
            n --;
        }
        else {
            n = half;
        }
    }
    /*qDebug("closest snapshot for %s was %lli", qPrintable(timestamp.to_string()), (*begin)->get_snapshot_id());*/
    if(begin != internal_list.begin()) begin --;
    return *(begin);
#if 0
    Snapshot *list_shot = NULL;
    /* NOTE: speed this up via a binary-style search through the list: try size/2, size/2+size/4 or size/2-size/4 . . . */
    for(int i = 0; i < internal_list.size(); i ++) {
        if(timestamp >= internal_list[i]->get_timestamp())
            list_shot = internal_list[i];
    }
    
    return list_shot;
#endif
}

bool SnapshotList::move_snapshot_to_event(Snapshot *temporary_snapshot, int amount) {
    Snapshot *list_shot = get_closest_snapshot(temporary_snapshot->get_timestamp());
    QList<Event *> event_list = list_shot->get_event_list()->get_event_list();
    bool applied = false;
    while(amount) {
        for(int i = 0; i < event_list.size() && amount; i ++) {
            Event *event = event_list[i];
            if(event->get_timestamp() >= temporary_snapshot->get_timestamp() && 
                !temporary_snapshot->get_event_list()->get_event_list().contains(event)) {
                
                event->apply_to(temporary_snapshot);
                temporary_snapshot->add_event(event);
                applied = true;
                amount --;
            }
        }
        if(list_shot->get_snapshot_id()+1 > internal_list.back()->get_snapshot_id()) {
            break;
        }
        list_shot = get_snapshot(list_shot->get_snapshot_id()+1);
        event_list = list_shot->get_event_list()->get_event_list();
    }
    return applied;
}

int SnapshotList::count_events(const Timestamp& from, const Timestamp& to) {
    Snapshot *from_snapshot = get_closest_snapshot(from);
    Snapshot *to_snapshot = NULL;
    int count = 0;
    
    int i = 0;
    for(; i < from_snapshot->get_event_list()->get_event_list().size(); i ++) {
        if(from >= from_snapshot->get_event_list()->get_event_list()[i]->get_timestamp()) {
            count ++;
            break;
        }
    }
    to_snapshot = get_closest_snapshot(to);
    if(to_snapshot != from_snapshot) {
        for(SnapshotID j = from_snapshot->get_snapshot_id(); j < to_snapshot->get_snapshot_id(); j ++) {
            count += get_snapshot(j)->get_event_list()->get_event_list().count();
        }
        i = 0;
    }
    for(; i < to_snapshot->get_event_list()->get_event_list().size(); i ++) {
        if(to > to_snapshot->get_event_list()->get_event_list()[i]->get_timestamp()) {
            break;
        }
        count ++;
    }
    
    return count;
}
