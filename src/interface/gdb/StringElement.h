#ifndef AESALON_INTERFACE_GDB_STRING_ELEMENT_H
#define AESALON_INTERFACE_GDB_STRING_ELEMENT_H

#include "ParseElement.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class StringElement : public ParseElement {
private:
    std::string data;
public:
    StringElement(std::string data) : data(data) {}
    virtual ~StringElement() {}
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
