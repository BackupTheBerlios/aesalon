#ifndef AESALON_INTERFACE_GDB_TUPLE_ELEMENT_H
#define AESALON_INTERFACE_GDB_TUPLE_ELEMENT_H

#include "ParseElement.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class TupleElement : public ParseElement {
private:
    ParseElementWrapper tuple_head;
public:
    TupleElement() {}
    virtual ~TupleElement() {}
    
    ParseElementWrapper get_tuple_head() const { return tuple_head; }
    void set_tuple_head(ParseElementWrapper new_tuple_head) { tuple_head = new_tuple_head; }
    void add_tuple_item(ParseElementWrapper new_tuple_item);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
