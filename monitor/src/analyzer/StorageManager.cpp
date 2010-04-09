#include <sys/mman.h>
#include "StorageManager.h"

namespace Analyzer {

StorageManager::StorageManager() {
    storage_size = 1048576;
    storage = static_cast<Byte *>(mmap(NULL, storage_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
}

StorageManager::~StorageManager() {
    for(object_list_t::iterator i = object_list.begin(); i != object_list.end(); i ++) {
        (*i)->~StorageObject();
    }
    munmap(storage, storage_size);
}

StorageItem *StorageManager::new_item() {
    StorageOffset offset = reserve(sizeof(StorageItem));
    StorageItem *item = new(storage + offset) StorageItem();
    item->set_storage_offset(offset);
    object_list.push_back(item);
    return item;
}

StorageAttribute *StorageManager::new_attribute() {
    StorageOffset offset = reserve(sizeof(StorageAttribute));
    StorageAttribute *attribute = new(storage + offset) StorageAttribute();
    attribute->set_storage_offset(offset);
    object_list.push_back(attribute);
    return attribute;
}

StorageOffset StorageManager::reserve(StorageOffset size) {
    StorageOffset final_offset = used_storage + size;
    StorageOffset original_offset = used_storage;
    if(final_offset > storage_size) {
        StorageOffset original_size = storage_size;
        while(storage_size < final_offset) storage_size *= 2;
        storage = static_cast<Byte *>(mremap(storage, original_size, storage_size, MREMAP_MAYMOVE));
    }
    used_storage = final_offset;
    return original_offset;
}

} // namespace Analyzer
