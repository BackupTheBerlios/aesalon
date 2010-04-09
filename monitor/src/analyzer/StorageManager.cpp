#include <sys/mman.h>
#include <cstring>
#include <iostream>
#include "StorageManager.h"

namespace Analyzer {

StorageManager::StorageManager() {
    storage_size = 1048576;
    storage = static_cast<Byte *>(mmap(NULL, storage_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    
    used_storage = 1;
}

StorageManager::~StorageManager() {
    munmap(storage, storage_size);
}

StorageOffset StorageManager::new_attribute() {
    StorageOffset offset = reserve(sizeof(StorageAttribute));
    StorageAttribute *attribute = reinterpret_cast<StorageAttribute *>(storage + offset);
    attribute->offset = offset;
    attribute->next = -1;
    attribute->child = -1;
    attribute->name = 0;
    attribute->value = 0;
    return offset;
}

StorageOffset StorageManager::new_string(std::string string) {
    std::size_t length = string.length() * sizeof(char);
    StorageOffset offset = reserve(length + sizeof(char));
    std::memcpy(storage + offset, string.c_str(), length + 1);
    return offset;
}

StorageOffset StorageManager::new_string(const char *string) {
    std::size_t length = strlen(string) * sizeof(char);
    StorageOffset offset = reserve(length + sizeof(char));
    std::memcpy(storage + offset, string, length + 1);
    return offset;
}

StorageOffset StorageManager::new_string(StorageOffset size) {
    StorageOffset offset = reserve(size);
    return offset;
}

StorageAttribute *StorageManager::dereference_attribute(StorageOffset offset) const {
    return reinterpret_cast<StorageAttribute *>(storage + offset);
}

char *StorageManager::dereference_string(StorageOffset offset) const {
    return reinterpret_cast<char *>(storage + offset);
}

StorageAttribute *StorageManager::create_child(StorageOffset offset, const char *name) { 
    StorageOffset attribute_offset = new_attribute();
    dereference_attribute(attribute_offset)->name = new_string(name);
    
    StorageAttribute *attribute = dereference_attribute(offset);
    offset = attribute->child;
    if(offset == -1) {
        attribute->child = attribute_offset;
    }
    else {
        while(offset != -1) {
            attribute = dereference_attribute(offset);
            offset = attribute->next;
        }
        attribute->next = attribute_offset;
    }
    
    return dereference_attribute(attribute_offset);
}

StorageOffset StorageManager::get_child(StorageOffset offset, const char *name) const {
    return get_from_list(dereference_attribute(offset)->child, name);
}

StorageOffset StorageManager::get_from_list(StorageOffset offset, const char *name) const {
    if(offset == -1) {
        return -1;
    }
    while(offset != -1) {
        StorageAttribute *attribute = dereference_attribute(offset);
        char *attribute_name = dereference_string(attribute->name);
        if(strcmp(attribute_name, name) == 0) {
            return offset;
        }
        
        offset = attribute->next;
    }
    return -1;
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
