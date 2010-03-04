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
