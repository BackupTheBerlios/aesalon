#include <QTimer>

#include "ActiveSessionMemory.h"
#include "ActiveSessionMemory.moc"

void ActiveSessionMemorySnapshot::add_block(quint64 address, quint64 size) {
    content.insert(storage->alloc_new_block(address, size)->get_offset());
}

void ActiveSessionMemorySnapshot::add_block(ActiveSessionMemoryBlock *block) {
    content.insert(block->get_offset());
}

ActiveSessionMemoryBlock *ActiveSessionMemorySnapshot::get_block(StorageOffset offset) {
    return storage->get_block_at(offset);
}

void ActiveSessionMemorySnapshot::remove_block(ActiveSessionMemoryBlock *block) {
    if(block == NULL) return;
    content.remove(block->get_offset());
}

ActiveSessionMemory::ActiveSessionMemory(QObject *parent) : QObject(parent), storage(NULL), current_memory(NULL), current_changes(NULL) {
    storage = new ActiveSessionMemoryStorage(ActiveSessionMemoryStorage::ALLOC_MODE_1M);
    
    current_memory = new ActiveSessionMemorySnapshot(storage, -1, QDateTime::currentDateTime());
    current_changes = storage->alloc_new_snapshot();
}

ActiveSessionMemory::~ActiveSessionMemory() {
    delete storage;
    delete current_memory;
}

quint64 ActiveSessionMemory::pop_uint64() {
    if(unprocessed.size() < 8) return -1;
    QByteArray data = unprocessed.left(8);
    unprocessed.remove(0, 8);
    quint64 value = 0;
    /* NOTE: this is an unrolled loop, doesn't have to be like this. */
    value |= (uchar)data[7];
    value <<= 8;
    value |= (uchar)data[6];
    value <<= 8;
    value |= (uchar)data[5];
    value <<= 8;
    value |= (uchar)data[4];
    value <<= 8;
    value |= (uchar)data[3];
    value <<= 8;
    value |= (uchar)data[2];
    value <<= 8;
    value |= (uchar)data[1];
    value <<= 8;
    value |= (uchar)data[0];
    return value;
}

void ActiveSessionMemory::process_data(QByteArray data) {
#if 0
    unprocessed += data;
    while(unprocessed.size()) {
        ActiveSessionMemoryCommand *command = NULL;
        char type = unprocessed[0];
        unprocessed.remove(0, 1);
        if(type & 0x01) {
            /* block event */
            quint64 address = 0;
            int block_type = (type & 0x06) >> 1;
            address = pop_uint64();
            /* block type is zero for allocations */
            if(block_type == 0) {
                quint64 size = pop_uint64();
                command = new ActiveSessionMemoryBlockAllocation(QDateTime::currentDateTime(), address, size);
            }
            /* block type is one for reallocations */
            else if(block_type == 1) {
                quint64 new_size = pop_uint64();
                quint64 new_address = pop_uint64();
                ActiveSessionMemoryBlock *block = get_current_snapshot()->get_block(address);
                if(!block) continue;
                command = new ActiveSessionMemoryBlockReallocation(QDateTime::currentDateTime(), address, new_address, block->get_size(), new_size);
            }
            /* block type is two for deallocations */
            else if(block_type == 2) {
                ActiveSessionMemoryBlock *block = get_current_snapshot()->get_block(address);
                if(!block) continue;
                command = new ActiveSessionMemoryBlockDeallocation(QDateTime::currentDateTime(), address, block->get_size());
            }
            /* block type three is unused, also known as an error . . . */
        }
        if(command == NULL) continue;
        /* Now, apply it to the current snapshot . . . */
        command->apply_to(get_current_snapshot());
        /* And push it onto the current-event queue. */
        current_queue.enqueue(command);
    }
    /* If it's more than 1000 events on the queue, then push the current snapshot onto the snapshot queue and clear the event queue . . . */
    if(current_queue.size() > 1000) {
        snapshot_queue.enqueue(get_current_snapshot()->clone());
        current_queue.clear();
    }
    /* Store a maximum of 10 snapshots on the snapshot queue . . . */
    if(snapshot_queue.size() >= 10) {
        ActiveSessionMemorySnapshot *snapshot = new ActiveSessionMemorySnapshot();
        snapshot_queue[snapshot_queue.size()-1]->apply_to(snapshot);
        snapshot_queue.clear();
        unbounded_snapshot_queue.enqueue(snapshot);
    }
    emit memory_changed(get_current_snapshot());
#endif
}
