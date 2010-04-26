#include <sys/mman.h>
#include <cstring>
#include <iostream>
#include "StorageManager.h"
#include "Initializer.h"

#ifndef CHUNK_SIZE
    #define CHUNK_SIZE 10485760
#endif

namespace Analyzer {

StorageManager::Chunk::Chunk(StorageOffset size) : total_size(size) {
    storage = static_cast<Byte *>(mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    used = 0;
}

StorageManager::Chunk::~Chunk() {
    munmap(storage, total_size);
}

Byte *StorageManager::Chunk::reserve(StorageOffset size) {
    Byte *ret = storage + used;
    used += size;
    return ret;
}

StorageManager::StorageManager() {
    std::stringstream stream;
    stream << Initializer::get_instance()->get_argument_parser()->get_argument("chunk-size")->get_data();
    stream >> chunk_size;
    if(chunk_size == 0) chunk_size = CHUNK_SIZE;
}

StorageManager::~StorageManager() {
    
}

StorageAttribute *StorageManager::new_attribute() {
    Byte *storage = reserve(sizeof(StorageAttribute));
    
    StorageAttribute *attribute = reinterpret_cast<StorageAttribute *>(new(storage) StorageAttribute());
    return attribute;
}

char *StorageManager::new_string(std::string string) {
    std::size_t length = string.length() * sizeof(char);
    
    Byte *storage = reserve(length + sizeof(char));
    std::memcpy(storage, string.c_str(), length + 1);
    return reinterpret_cast<char *>(storage);
}

char *StorageManager::new_string(const char *string) {
    std::size_t length = strlen(string) * sizeof(char);
    
    char *storage = (char *)reserve(length + sizeof(char));
    std::memcpy(storage, string, length + 1);
    return storage;
}

char *StorageManager::new_string(StorageOffset size) {
    Byte *storage = reserve(size);
    return reinterpret_cast<char *>(storage);
}

StorageAttribute *StorageManager::create_child(StorageAttribute *parent, const char *name) { 
    const char *string = new_string(name);
    
    StorageAttribute *attribute = new_attribute();
    attribute->set_name(string);
    
    StorageAttribute *child = parent->get_child();
    if(child == NULL) {
        parent->set_child(attribute);
    }
    else {
        while(child->get_next() != NULL) {
            child = child->get_next();
        }
        child->set_next(attribute);
    }
    
    return attribute;
}

StorageAttribute *StorageManager::get_child(StorageAttribute *parent, const char *name) const {
    return get_from_list(parent->get_child(), name);
}

StorageAttribute *StorageManager::get_from_list(StorageAttribute *head, const char *name) const {
    if(head == NULL) {
        return NULL;
    }
    while(head != NULL) {
        if(strcmp(head->get_name(), name) == 0) {
            return head;
        }
        
        head = head->get_next();
    }
    return NULL;
}

Byte *StorageManager::reserve(StorageOffset size) {
    if(size > chunk_size) {
        chunk_list.push_back(new Chunk(size));
        return chunk_list.back()->reserve(size);
    }
    for(chunk_list_t::iterator i = chunk_list.begin(); i != chunk_list.end(); i ++) {
        if((*i)->get_free() >= size) return (*i)->reserve(size);
    }
    
    chunk_list.push_back(new Chunk(chunk_size));
    return chunk_list.back()->reserve(size);
}

} // namespace Analyzer
