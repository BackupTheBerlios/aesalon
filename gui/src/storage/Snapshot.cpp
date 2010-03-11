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
    
    @file storage/Snapshot.cpp
*/

#include <QQueue>
#include "Snapshot.h"

/* NOTE: 56 shouldn't be hardcoded! It needs to be stored inside the DataSource, I think. */
Snapshot::Snapshot(SnapshotID snapshot_id) : snapshot_id(snapshot_id), head_node(NULL), max_tree_depth(56), block_count(0) {

}

Snapshot::~Snapshot() {

}

void Snapshot::free_tree() {
    QQueue<BiTreeNode *> to_visit;
    QList<BiTreeNode *> to_delete;
    
    if(head_node == NULL) return;
    to_visit.enqueue(head_node->get_left());
    to_visit.enqueue(head_node->get_right());
    
    while(to_visit.size()) {
        BiTreeNode *node = to_visit.dequeue();
        if(node == NULL) continue;
        to_visit.enqueue(node->get_left());
        to_visit.enqueue(node->get_right());
        if(node->get_snapshot_id() == get_snapshot_id()) to_delete.append(node);
    }
    foreach(BiTreeNode *node, to_delete) {
        delete node;
    }
}
