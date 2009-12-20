#include "Tag.h"

namespace Aesalon {
namespace Interface {
namespace DWARF {

Misc::SmartPointer<Attribute> Tag::get_attribute(std::string name) const {
    for(attribute_list_t::const_iterator i = attribute_list.begin(); i != attribute_list.end(); i ++) {
        if((*i)->get_name() == name) return *i;
    }
    return NULL;
}

} // namespace DWARF
} // namespace Interface
} // namespace Aesalon
