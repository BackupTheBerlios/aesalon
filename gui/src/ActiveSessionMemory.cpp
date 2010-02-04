#include <QTimer>

#include "ActiveSessionMemory.h"
#include "ActiveSessionMemory.moc"

void ActiveSessionMemorySnapshot::add_block(quint64 address, quint64 size) {
    qDebug("Adding block with address %llu, size %llu", address, size);
    content.append(storage->alloc_new_block(address, size)->get_offset());
}

void ActiveSessionMemorySnapshot::add_block(ActiveSessionMemoryBlock *block) {
    qDebug("Adding block at %p -- offset is %li", block, block->get_offset());
    content.append(block->get_offset());
}

void ActiveSessionMemorySnapshot::add_block(StorageOffset offset) {
    qDebug("Adding block with offset %li", offset);
    content.append(offset);
}

ActiveSessionMemoryBlock *ActiveSessionMemorySnapshot::get_block(quint64 address) const {
    foreach(StorageOffset offset, content) {
        if(storage->get_offset_data_type(offset) == ActiveSessionMemoryStorage::BLOCK_DATA) {
            if(storage->get_block_at(offset)->get_address() == address) return storage->get_block_at(offset);
        }
        else if(storage->get_offset_data_type(offset) == ActiveSessionMemoryStorage::SNAPSHOT_DATA) {
            ActiveSessionMemoryBlock *block = storage->get_snapshot_at(offset)->get_block(address);
            if(block) return block;
        }
    }
    return NULL;
}

void ActiveSessionMemorySnapshot::remove_block(ActiveSessionMemoryBlock *block) {
    if(block == NULL) return;
    content.removeAt(content.indexOf(block->get_offset()));
}

void ActiveSessionMemorySnapshot::copy_into(ActiveSessionMemorySnapshot *snapshot) const {
    snapshot->content = content;
    snapshot->allocations = allocations;
    snapshot->reallocations = reallocations;
    snapshot->deallocations = deallocations;
    snapshot->timestamp = timestamp;
}

void ActiveSessionMemorySnapshot::assemble_from(ActiveSessionMemorySnapshot *snapshot, bool remove) {
    foreach(StorageOffset offset, snapshot->content) {
        ActiveSessionMemoryStorage::data_type_e data_type = storage->get_offset_data_type(qAbs<StorageOffset>(offset));
        if(data_type == ActiveSessionMemoryStorage::SNAPSHOT_DATA) {
            qDebug("Chaining on to snapshot at offset %llu . . .", qAbs<StorageOffset>(offset));
            assemble_from(storage->get_snapshot_at(qAbs<StorageOffset>(offset)), (offset<0 || remove));
        }
        else if(data_type == ActiveSessionMemoryStorage::BLOCK_DATA) {
            qDebug("Inserting content offset %i . . .", offset * (remove?-1:1));
            content.append(offset * (remove?-1:1));
        }
        else qDebug("Unknown data type encountered for offset %i . . .", offset);
    }
}

void ActiveSessionMemorySnapshot::dump_content() {
    qDebug("Dumping content of offset at %li . . .", get_offset());
    foreach(StorageOffset offset, content) {
        switch(storage->get_offset_data_type(qAbs<StorageOffset>(offset))) {
            case ActiveSessionMemoryStorage::BLOCK_DATA:
                qDebug("\tStored block at %li", offset);
                break;
            case ActiveSessionMemoryStorage::SNAPSHOT_DATA:
                qDebug("\tStored snapshot at %li", offset);
                break;
        }
    }
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
    /* In order to "save" the current snapshot, all that is required is to just
        allocate a new "changed" snapshot. The old one's already stored. However,
        store a "full" snapshot once every max_partial_snapshots . . . */
    StorageOffset last_snapshot = current_changes->get_offset();
    if(!(snapshot_list.size() % session->get_full_snapshot_interval())) {
        last_snapshot = storage->copy_snapshot(current_memory->get_offset())->get_offset();
    }
    qDebug("dumping current_changes . . .");
    current_changes->set_timestamp(QDateTime::currentDateTime());
    current_changes->dump_content();
    snapshot_list.append(current_changes->get_offset());
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
                qDebug("Adding remove offset %li to current_changes . . .", -block->get_offset());
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

ActiveSessionMemorySnapshot *ActiveSessionMemory::get_snapshot_for(QDateTime time) const {
    qDebug("Assembling snapshot for date/time %s . . .", time.toString().toStdString().c_str());
    ActiveSessionMemorySnapshot *snapshot = storage->alloc_new_snapshot();
    ActiveSessionMemorySnapshot *last_snapshot = NULL;
    
    qDebug("snapshot_list.size(): %i", snapshot_list.size());
    
    int i;
    for(i = snapshot_list.size()-1; i > 0; i --) {
        last_snapshot = storage->get_snapshot_at(snapshot_list[i]);
        if(last_snapshot->get_timestamp().secsTo(time) >= 0) break;
        qDebug("Passing over %s . . .", last_snapshot->get_timestamp().toString().toStdString().c_str());
    }
    if(i == -1) return snapshot;
    qDebug("Assembling from snapshot at %s.", last_snapshot->get_timestamp().toString().toStdString().c_str());
    snapshot->assemble_from(last_snapshot);
    
    return snapshot;
}
