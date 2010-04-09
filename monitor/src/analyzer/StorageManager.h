#ifndef AESALON_ANALYZER_STORAGE_MANAGER_H

#include <list>

#include "StorageOffset.h"
#include "StorageItem.h"
#include "StorageAttribute.h"

namespace Analyzer {

class StorageManager {
private:
    typedef std::list<StorageObject *> object_list_t;
    object_list_t object_list;
    
    Byte *storage;
    StorageOffset storage_size;
    StorageOffset used_storage;
public:
    StorageManager();
    ~StorageManager();
    
    StorageItem *new_item();
    StorageAttribute *new_attribute();
private:
    StorageOffset reserve(StorageOffset size);
};

} // namespace Analyzer

#endif