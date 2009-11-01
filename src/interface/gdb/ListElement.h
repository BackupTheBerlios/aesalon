#ifndef AESALON_INTERFACE_GDB_LIST_ELEMENT_H
#define AESALON_INTERFACE_GDB_LIST_ELEMENT_H

#include "ParseElement.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class ListElement : public ParseElement {
public:
    ListElement() {}
    virtual ~ListElement() {}
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
