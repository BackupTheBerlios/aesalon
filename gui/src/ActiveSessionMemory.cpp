#include <QTimer>

#include "ActiveSessionMemory.h"
#include "ActiveSessionMemory.moc"

void ActiveSessionMemorySnapshot::add_block(quint64 address, quint64 size) {
    content.insert(storage->alloc_new_block(address, size)->get_offset());
}

void ActiveSessionMemorySnapshot::add_block(ActiveSessionMemoryBlock *block) {
    content.insert(block->get_offset());
}

void ActiveSessionMemorySnapshot::add_block(StorageOffset offset) {
    content.insert(offset);
}

ActiveSessionMemoryBlock *ActiveSessionMemorySnapshot::get_block(StorageOffset offset) const {
    return storage->get_block_at(offset);
}

void ActiveSessionMemorySnapshot::remove_block(ActiveSessionMemoryBlock *block) {
    if(block == NULL) return;
    content.remove(block->get_offset());
}

void ActiveSessionMemorySnapshot::copy_into(ActiveSessionMemorySnapshot *snapshot) const {
    snapshot->content = content;
    snapshot->allocations = allocations;
    snapshot->reallocations = reallocations;
    snapshot->deallocations = deallocations;
    snapshot->timestamp = timestamp;
}

ActiveSessionMemory::ActiveSessionMemory(QObject *parent, Session *session)
    : QObject(parent), session(session), storage(NULL), current_memory(NULL), current_changes(NULL) {
    storage = new ActiveSessionMemoryStorage(ActiveSessionMemoryStorage::ALLOC_MODE_1M);
    
    current_memory = storage->alloc_new_snapshot();
    /*current_memory = new ActiveSessionMemorySnapshot(storage, -1, QDateTime::currentDateTime());*/
    current_changes = storage->alloc_new_snapshot();
    
    snapshot_timer = new QTimer(this);
    connect(snapshot_timer, SIGNAL(timeout()), this, SLOT(save_snapshot()));
    snapshot_timer->start(session->get_snapshot_interval());
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

void ActiveSessionMemory::save_snapshot() {
    qDebug("Saving snapshot . . .");
    /* In order to "save" the current snapshot, all that is required is to just
        allocate a new "changed" snapshot. The old one's already stored. However,
        store a "full" snapshot once every max_partial_snapshots . . . */
    StorageOffset last_snapshot = current_changes->get_offset();
    if(!(snapshot_list.size() % session->get_full_snapshot_interval())) {
        last_snapshot = storage->copy_snapshot(current_memory->get_offset())->get_offset();
    }
    current_changes = storage->alloc_new_snapshot();
    current_changes->add_block(last_snapshot);
}

void ActiveSessionMemory::started(QDateTime time) {
    snapshot_timer->start(session->get_snapshot_interval());
}

void ActiveSessionMemory::finished(QDateTime time) {
    snapshot_timer->stop();
    save_snapshot();
}

void ActiveSessionMemory::process_data(QByteArray data) {
    enum {
        /* block type is zero for allocations */
        BLOCK_EVENT_ALLOC = 0,
        /* block type is one for reallocations */
        BLOCK_EVENT_REALLOC,
        /* block type is two for deallocations */
        BLOCK_EVENT_FREE
    };
    unprocessed += data;
    while(unprocessed.size()) {
        char type = unprocessed[0];
        unprocessed.remove(0, 1);
        if(type & 0x01) {
            /* block event */
            quint64 address = 0;
            int block_type = (type & 0x06) >> 1;
            address = pop_uint64();
            if(block_type == BLOCK_EVENT_ALLOC) {
                quint64 size = pop_uint64();
                ActiveSessionMemoryBlock *block = storage->alloc_new_block(address, size);
                current_memory->add_block(block);
                current_changes->add_block(block);
                current_memory->inc_allocations();
                current_changes->inc_allocations();
            }
            else if(block_type == BLOCK_EVENT_REALLOC) {
                quint64 new_size = pop_uint64();
                quint64 new_address = pop_uint64();
                ActiveSessionMemoryBlock *block = current_memory->get_block(address);
                if(!block) continue;
                current_memory->remove_block(block);
                current_changes->add_block(-block->get_offset());
                block = storage->alloc_new_block(new_address, new_size);
                current_memory->add_block(block);
                current_changes->add_block(block);
                current_memory->inc_reallocations();
                current_changes->inc_reallocations();
            }
            else if(block_type == BLOCK_EVENT_FREE) {
                ActiveSessionMemoryBlock *block = current_memory->get_block(address);
                if(!block) continue;
                current_memory->remove_block(block);
                current_changes->add_block(-block->get_offset());
                current_memory->inc_deallocations();
                current_changes->inc_deallocations();
            }
            /* block type three is unused, and is also known as an error . . . */
            current_memory->set_timestamp(QDateTime::currentDateTime());
        }
    }
    
    emit memory_changed(get_current_memory());
}
