#ifndef AESALON_GUI_STORAGE_FACTORY_H
#define AESALON_GUI_STORAGE_FACTORY_H

#include <QMutex>
#include "Snapshot.h"
#include "SnapshotID.h"
#include "BiTreeNode.h"
#include "Block.h"

class StorageFactory {
public:
    static Snapshot *new_snapshot() {
        static QMutex snapshot_id_mutex;
        static SnapshotID last_snapshot_id = 0;
        QMutexLocker locker(&snapshot_id_mutex);
        return new Snapshot(++last_snapshot_id);
    }
    static BiTreeNode *new_node(SnapshotID snapshot_id) {
        return new BiTreeNode(snapshot_id);
    }
    static Block *new_block(MemoryAddress address, MemorySize size) {
        return new Block(address, size);
    }
};

#endif
