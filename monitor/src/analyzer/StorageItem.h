#ifndef AESALON_ANALYZER_STORAGE_ITEM_H
#define AESALON_ANALYZER_STORAGE_ITEM_H

#include <list>
#include <string>

#include "StorageObject.h"
#include "StorageAttribute.h"

namespace Analyzer {

class StorageItem : public StorageObject {
private:
    typedef std::list<StorageAttribute *> attribute_list_t;
    attribute_list_t attribute_list;
public:
    StorageItem();
    virtual ~StorageItem();
    
    StorageAttribute *get_attribute(const std::string &name);
    void add_attribute(StorageAttribute *attribute);
};

} // namespace Analyzer

#endif
