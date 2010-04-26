#ifndef AESALON_ANALYZER_STORAGE_ATTRIBUTE_H
#define AESALON_ANALYZER_STORAGE_ATTRIBUTE_H

#include "StorageOffset.h"

namespace Analyzer {

class StorageAttribute {
private:
    const char *name;
    Word value;
    StorageAttribute *next;
    StorageAttribute *child;
public:
    StorageAttribute();
    ~StorageAttribute();
    
    const char *get_name() const { return name; }
    void set_name(const char *new_name) { name = new_name; }
    
    Word get_value() const { return value; }
    void set_value(Word new_value) { value = new_value; }
    StorageAttribute *get_next() const { return next; }
    void set_next(StorageAttribute *new_next) { next = new_next; }
    StorageAttribute *get_child() const { return child; }
    void set_child(StorageAttribute *new_child) { child = new_child; }
};

} // namespce Analyzer

#endif
