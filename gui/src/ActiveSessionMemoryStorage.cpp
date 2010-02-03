#include <sys/mman.h>
#include <errno.h>

#include "ActiveSessionMemoryStorage.h"

ActiveSessionMemoryStorage::ActiveSessionMemoryStorage(allocation_mode_e allocation_mode)
    : data(NULL), data_size(0), unused_offset(0), allocation_mode(allocation_mode) {
    
    data = static_cast<quint8 *>(mmap(NULL, ((allocation_mode == ALLOC_MODE_1M)?1048276:1), 
        PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
}

ActiveSessionMemoryStorage::~ActiveSessionMemoryStorage() {
    if(data != NULL) {
        munmap(data, data_size);
    }
}

void ActiveSessionMemoryStorage::reserve_more_memory() {
    quint64 amount = (allocation_mode == ALLOC_MODE_1M)?(1048576):(data_size);
    data = static_cast<quint8 *>(mremap(static_cast<void *>(data), data_size, data_size + amount, MREMAP_MAYMOVE));
    if(data == MAP_FAILED) {
        qCritical("Failed to resize memory to size 0x%p!", data_size + amount);
    }
    data_size += amount;
}
