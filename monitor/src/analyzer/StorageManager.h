#ifndef AESALON_ANALYZER_STORAGE_MANAGER_H

#include <string>

#include "StorageOffset.h"
#include "StorageAttribute.h"

namespace Analyzer {

class StorageManager {
private:
    Byte *storage;
    StorageOffset storage_size;
    StorageOffset used_storage;
public:
    StorageManager();
    ~StorageManager();
    
    StorageOffset new_attribute();
    StorageOffset new_string(std::string string);
    StorageOffset new_string(const char *string);
    StorageOffset new_string(StorageOffset size);
    
    StorageAttribute *dereference_attribute(StorageOffset offset) const;
    char *dereference_string(StorageOffset offset) const;
    
    StorageAttribute *create_child(StorageOffset offset, const char *name);
    StorageOffset get_child(StorageOffset offset, const char *name) const;
    StorageOffset get_from_list(StorageOffset offset, const char *name) const;
private:
    StorageOffset reserve(StorageOffset size);
};

} // namespace Analyzer

#endif