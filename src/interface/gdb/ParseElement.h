#ifndef AESALON_INTERFACE_GDB_PARSE_ELEMENT_H
#define AESALON_INTERFACE_GDB_PARSE_ELEMENT_H

#include <vector>
#include <cstddef>

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class ParseElement;

class ParseElementWrapper : public Misc::SmartPointer<ParseElement> {
public:
    ParseElementWrapper(ParseElement *element = NULL) : Misc::SmartPointer<ParseElement>(element) {}
    virtual ~ParseElementWrapper() {}
};

class ParseElement {
private:
    ParseElementWrapper next, prev;
    ParseElementWrapper parent;
public:
    ParseElement() {}
    virtual ~ParseElement() {}
    
    ParseElementWrapper get_next() const { return next; }
    void set_next(ParseElementWrapper new_next) { next = new_next; }
    ParseElementWrapper get_prev() const { return prev; }
    void set_prev(ParseElementWrapper new_prev) { prev = new_prev; }
    ParseElementWrapper get_parent() const { return parent; }
    void set_parent(ParseElementWrapper new_parent) { parent = new_parent; }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
