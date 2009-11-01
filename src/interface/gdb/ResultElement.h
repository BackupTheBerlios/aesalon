#ifndef AESALON_INTERFACE_GDB_RESULT_ELEMENT_H
#define AESALON_INTERFACE_GDB_RESULT_ELEMENT_H

#include "ParseElement.h"
#include "StringElement.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class ResultElement : public ParseElement {
private:
    ParseElementWrapper variable_name;
    ParseElementWrapper value;
public:
    ResultElement(ParseElementWrapper variable_name, ParseElementWrapper value) : variable_name(variable_name), value(value) {}
    virtual ~ResultElement() {}
    
    ParseElementWrapper get_variable_name() const { return variable_name; }
    void set_variable_name(ParseElementWrapper new_variable_name) { variable_name = new_variable_name; }
    ParseElementWrapper get_value() const { return value; }
    void set_value(ParseElementWrapper new_value) { value = new_value; }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
