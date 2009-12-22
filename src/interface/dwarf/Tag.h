#ifndef AESALON_INTERFACE_DWARF_TAG_H
#define AESALON_INTERFACE_DWARF_TAG_H

#include <string>
#include <map>

#include "Attribute.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
namespace DWARF {

class Tag {
public:
    typedef std::map<std::string, Misc::SmartPointer<Attribute> > attribute_map_t;
private:
    std::string name;
    
    attribute_map_t attribute_map;
public:
    Tag(std::string name) : name(name) {}
    virtual ~Tag() {}
    
    std::string get_name() const { return name; }
    
    Misc::SmartPointer<Attribute> get_attribute(std::string name) const { return attribute_map.at(name); }
};

} // namespace DWARF
} // namespace Interface
} // namespace Aesalon

#endif
