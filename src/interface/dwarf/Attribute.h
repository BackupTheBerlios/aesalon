#ifndef AESALON_INTERFACE_DWARF_ATTRIBUTE_H
#define AESALON_INTERFACE_DWARF_ATTRIBUTE_H

#include <string>

#include "AttributeData.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
namespace DWARF {

class Attribute {
private:
    std::string name;
    Misc::SmartPointer<AttributeData> data;
public:
    Attribute(std::string name) : name(name) {}
    virtual ~Attribute() {}
    
    std::string get_name() const { return name; }
    
    Misc::SmartPointer<AttributeData> get_data() const { return data; }
};

} // namespace DWARF
} // namespace Interface
} // namespace Aesalon

#endif
