#include "Snapshot.h"

/* NOTE: 56 shouldn't be hardcoded! It needs to be stored inside the DataSource, I think. */
Snapshot::Snapshot(SnapshotID snapshot_id) : snapshot_id(snapshot_id), head_node(NULL), max_tree_depth(56) {

}

Snapshot::~Snapshot() {

}
