#ifndef AESALON_INTERFACE_GDB_PARSE_ELEMENT_H
#define AESALON_INTERFACE_GDB_PARSE_ELEMENT_H

#include <vector>
#include <cstddef>

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class ParseElement;

typedef Misc::SmartPointer<ParseElement> ParseElementWrapper;

class ParseElement {
public:
    typedef std::vector<ParseElementWrapper> parse_element_vector_t;
private:
    parse_element_vector_t parse_element_vector;
public:
    ParseElement();
    virtual ~ParseElement();
    
    ParseElementWrapper get_parse_element(std::size_t which) const
        { return parse_element_vector.at(which); }
    std::size_t get_parse_elements() const
        { return parse_element_vector.size(); }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
