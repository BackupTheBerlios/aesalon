#ifndef AESALON_INTERFACE_DWARF_TAG_H
#define AESALON_INTERFACE_DWARF_TAG_H

#include <string>
#include <vector>

#include "Attribute.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
namespace DWARF {

class Tag {
public:
    typedef std::vector<Misc::SmartPointer<Attribute> > attribute_list_t;
private:
    std::string name;
    
    attribute_list_t attribute_list;
public:
    Tag(std::string name) : name(name) {}
    virtual ~Tag() {}
    
    std::string get_name() const { return name; }
    
    Misc::SmartPointer<Attribute> get_attribute(std::string name) const;
};

} // namespace DWARF
} // namespace Interface
} // namespace Aesalon

#endif
