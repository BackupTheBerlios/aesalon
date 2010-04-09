#ifndef AESALON_ANALYZER_STORAGE_ATTRIBUTE_H
#define AESALON_ANALYZER_STORAGE_ATTRIBUTE_H

#include <string>

#include "StorageObject.h"
#include "StorageString.h"

namespace Analyzer {

class StorageAttribute : public StorageObject {
private:
    StorageString *name;
    Word value;
public:
    StorageAttribute();
    virtual ~StorageAttribute();
    
    StorageString *get_name() const { return name; }
    void set_name(StorageString *new_name) { name = new_name; }
    Word get_value() const { return value; }
    void set_value(Word new_value) { value = new_value; }
};

} // namespce Analyzer

#endif
