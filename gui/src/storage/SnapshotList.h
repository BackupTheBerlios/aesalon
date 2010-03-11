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
    
    @file storage/SnapshotList.h
*/

#ifndef AESALON_GUI_STORAGE_SNAPSHOT_LIST_H
#define AESALON_GUI_STORAGE_SNAPSHOT_LIST_H

#include <QList>

#include "SnapshotID.h"
#include "Snapshot.h"
#include "Timestamp.h"
#include "EventVisitor.h"

class SnapshotList {
private:
    SnapshotID last_id;
    SnapshotID last_temporary_id;
    QList<Snapshot *> internal_list;
public:
    SnapshotList();
    virtual ~SnapshotList();
    
    SnapshotID get_last_id() const { return last_id; }
    SnapshotID alloc_temporary_id() { return --last_temporary_id; }
    
    Snapshot *append_snapshot();
    Snapshot *get_snapshot(SnapshotID id) const { return internal_list[id-1]; }
    Snapshot *get_snapshot_for(const Timestamp &timestamp);
    Snapshot *get_closest_snapshot(const Timestamp &timestamp);
    int get_snapshot_count() const { return internal_list.size(); }
    
    /* NOTE: can only adjust forwards in time, not backwards 
    void adjust_temporary_snapshot(Snapshot *temporary_snapshot, const Timestamp &to_timestamp);*/
    bool move_snapshot_to_event(Snapshot *temporary_snapshot, int amount);
    
    void iterate_through(const Timestamp &from, const Timestamp &to, EventVisitor &visitor);
    
    int count_events(const Timestamp &from, const Timestamp &to);
};

#endif
