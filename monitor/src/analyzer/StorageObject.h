#ifndef AESALON_ANALYZER_STORAGE_OBJECT_H
#define AESALON_ANALYZER_STORAGE_OBJECT_H

#include "StorageOffset.h"

namespace Analyzer {

class StorageObject {
private:
    StorageOffset storage_offset;
public:
    virtual ~StorageObject() {}
    
    StorageOffset get_storage_offset() const { return storage_offset; }
    void set_storage_offset(StorageOffset new_offset) { storage_offset = new_offset; }
};

} // namespace Analyzer

#endif
