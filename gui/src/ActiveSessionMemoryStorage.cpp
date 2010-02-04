#include <sys/mman.h>
#include <errno.h>

#include "ActiveSessionMemoryStorage.h"
#include "ActiveSessionMemory.h"

ActiveSessionMemoryStorage::ActiveSessionMemoryStorage(allocation_mode_e allocation_mode)
    : data(NULL), data_size(0), unused_offset(0), allocation_mode(allocation_mode) {
    
    data = static_cast<quint8 *>(mmap(NULL, ((allocation_mode == ALLOC_MODE_1M)?1048276:1), 
        PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
}

ActiveSessionMemoryStorage::~ActiveSessionMemoryStorage() {
    for(QMap<StorageOffset, data_type_e>::iterator i = data_types.begin(); i != data_types.end(); i ++) {
        switch(i.value()) {
            case BLOCK_DATA:
                get_block_at(i.key())->~ActiveSessionMemoryBlock();
                break;
            case SNAPSHOT_DATA:
                get_snapshot_at(i.key())->~ActiveSessionMemorySnapshot();
                break;
            case INVALID_DATA:
            default:
                break;
        }
    }
    if(data != NULL) {
        munmap(data, data_size);
    }
}

void ActiveSessionMemoryStorage::reserve_more_memory() {
    quint64 amount = (allocation_mode == ALLOC_MODE_1M)?(1048576):(data_size);
    data = static_cast<quint8 *>(mremap(static_cast<void *>(data), data_size, data_size + amount, MREMAP_MAYMOVE));
    if(data == MAP_FAILED) {
        qCritical("Failed to resize memory to size 0x%p!", data_size + amount);
        return;
    }
    data_size += amount;
}

ActiveSessionMemoryBlock *ActiveSessionMemoryStorage::alloc_new_block(quint64 address, quint64 size) {
    while((data_size - unused_offset) < sizeof(ActiveSessionMemoryBlock)) reserve_more_memory();
    ActiveSessionMemoryBlock *block = new(data + unused_offset) ActiveSessionMemoryBlock(unused_offset, address, size);
    unused_offset += sizeof(ActiveSessionMemoryBlock);
    return block;
}

ActiveSessionMemorySnapshot *ActiveSessionMemoryStorage::alloc_new_snapshot() {
    while((data_size - unused_offset) < sizeof(ActiveSessionMemorySnapshot)) reserve_more_memory();
    ActiveSessionMemorySnapshot *snapshot = new(data + unused_offset) ActiveSessionMemorySnapshot(this, unused_offset, QDateTime::currentDateTime());
    unused_offset += sizeof(ActiveSessionMemorySnapshot);
    return snapshot;
}

ActiveSessionMemorySnapshot *ActiveSessionMemoryStorage::copy_snapshot(StorageOffset offset) {
    while((data_size - unused_offset) < sizeof(ActiveSessionMemorySnapshot)) reserve_more_memory();
    ActiveSessionMemorySnapshot *snapshot = new(data + unused_offset) ActiveSessionMemorySnapshot(this, unused_offset, QDateTime::currentDateTime());
    unused_offset += sizeof(ActiveSessionMemorySnapshot);
    get_snapshot_at(offset)->copy_into(snapshot);
    return snapshot;
}

ActiveSessionMemoryBlock *ActiveSessionMemoryStorage::get_block_at(StorageOffset offset) {
    if(data_types[offset] != BLOCK_DATA) return NULL;
    return reinterpret_cast<ActiveSessionMemoryBlock *>(data + offset);
}

ActiveSessionMemorySnapshot *ActiveSessionMemoryStorage::get_snapshot_at(StorageOffset offset) { 
    if(data_types[offset] != SNAPSHOT_DATA) {
        qCritical("Asked to retrieve invalid snapshot!");
        return NULL;
    }
    return reinterpret_cast<ActiveSessionMemorySnapshot *>(data + offset);
}
