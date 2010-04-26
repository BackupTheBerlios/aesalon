#ifndef AESALON_ANALYZER_STORAGE_MANAGER_H
#define AESALON_ANALYZER_STORAGE_MANAGER_H

#include <string>
#include <vector>

#include "StorageOffset.h"
#include "StorageAttribute.h"

namespace Analyzer {

class StorageManager {
private:
    class Chunk {
    private:
        Byte *storage;
        StorageOffset used;
        StorageOffset total_size;
    public:
        Chunk(StorageOffset size);
        ~Chunk();
        
        Byte *get_storage() const { return storage; }
        
        Byte *reserve(StorageOffset size);
        
        StorageOffset get_used() const { return used; }
        StorageOffset get_free() const { return CHUNK_SIZE - used; }
    };
    typedef std::vector<Chunk *> chunk_list_t;
    chunk_list_t chunk_list;
    StorageOffset chunk_size;
public:
    StorageManager();
    ~StorageManager();
    
    StorageAttribute *new_attribute();
    char *new_string(std::string string);
    char *new_string(const char *string);
    char *new_string(StorageOffset size);
    
    StorageAttribute *create_child(StorageAttribute *parent, const char *name);
    StorageAttribute *get_child(StorageAttribute *parent, const char *name) const;
    StorageAttribute *get_from_list(StorageAttribute *head, const char *name) const;
private:
    Byte *reserve(StorageOffset size);
};

} // namespace Analyzer

#endif