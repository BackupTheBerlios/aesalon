#ifndef AESALON_ANALYZER_STORAGE_ATTRIBUTE_H
#define AESALON_ANALYZER_STORAGE_ATTRIBUTE_H

#include <string>

#include "StorageObject.h"

namespace Analyzer {

class StorageAttribute : public StorageObject {
private:
    StorageOffset name;
    Word value;
public:
    StorageAttribute();
    virtual ~StorageAttribute();
    
    StorageOffset get_name();
    Word get_value() const { return value; }
    void set_value(Word new_value) { value = new_value; }
};

} // namespce Analyzer

#endif
