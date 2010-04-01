/** Aesalon, a project to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009-2010 strange <kawk256@gmail.com>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    @file storage/SnapshotList.cpp
*/

#include <QList>
#include <QQueue>
#include <QtAlgorithms>
#include <algorithm>

#include "SnapshotList.h"
#include "Event.h"
#include "AllocEvent.h"
#include "FreeEvent.h"

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
}

Block *SnapshotList::get_block(const Timestamp &timestamp, MemoryAddress address) {
    Snapshot *snapshot = get_snapshot_for(timestamp);
    
    BiTreeNode *node = snapshot->get_head_node();
    
    qDebug("Traversing tree for %llx . . .", address);
    
    class Scanner {
    public:
        Block *look_for(MemoryAddress address, BiTreeNode *in) {
            if(in == NULL) return NULL;
            if(in->get_block_list_size()) return in->get_block(address);
            else {
                Block *block;
                if((block = look_for(address, in->get_left())) == NULL && (block = look_for(address, in->get_right())) == NULL) {}
                return block;
            }
        }
    };
    
    Scanner scanner;
    Block *b = scanner.look_for(address, node);
    return b;
    
    quint8 max_depth = snapshot->get_max_tree_depth();
    for(quint8 depth = 63; depth > 64 - max_depth; depth --) {
        quint64 mask = 0x01;
        mask <<= depth;
        
        if((address & mask) == 0) {
            if(node->get_left() == NULL) {
                qDebug("Couldn't traverse tree (depth is %i).", depth);
                return NULL;
            }
            node = node->get_left();
        }
        else {
            if(node->get_right() == NULL) {
                qDebug("Couldn't traverse tree (depth is %i).", depth);
                return NULL;
            }
            node = node->get_right();
        }
    }
    
    qDebug("Traversed tree, getting block . . .");
    
    Block *block = node->get_block(address);
    
    /*Block *block = node->get_block(address);
    qDebug("Returning %p . . .", block);*/
    return NULL;
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

void SnapshotList::iterate_through(const Timestamp &from, const Timestamp &to, EventVisitor &visitor) {
    Snapshot *snapshot = get_closest_snapshot(from);
    QList<Event *> event_list = snapshot->get_event_list()->get_event_list();
    int index = 0;
    for(; index < event_list.size(); index ++) {
        if(from <= event_list[index]->get_timestamp()) break;
    }
    bool finished = false;
    while(true) {
        for(; index < event_list.size(); index ++) {
            if(to < event_list[index]->get_timestamp()) {
                finished = true;
                break;
            }
            event_list[index]->accept(visitor);
        }
        if(finished || snapshot->get_snapshot_id() == internal_list.last()->get_snapshot_id()) break;
        snapshot = get_snapshot(snapshot->get_snapshot_id() + 1);
        event_list = snapshot->get_event_list()->get_event_list();
        index = 0;
    }
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
