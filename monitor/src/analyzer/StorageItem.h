#ifndef AESALON_ANALYZER_STORAGE_ITEM_H
#define AESALON_ANALYZER_STORAGE_ITEM_H

#include <map>
#include <string>

#include "StorageObject.h"
#include "StorageAttribute.h"

namespace Analyzer {

class StorageItem : public StorageObject {
private:
    std::map<std::string, StorageAttribute *> attribute_map;
public:
    StorageItem();
    virtual ~StorageItem();
    
    
};

} // namespace Analyzer

#endif
