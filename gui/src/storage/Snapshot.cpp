#include "Snapshot.h"

Snapshot::Snapshot(SnapshotID snapshot_id) : snapshot_id(snapshot_id), head_node(NULL), max_tree_depth(56) {

}

Snapshot::~Snapshot() {

}
