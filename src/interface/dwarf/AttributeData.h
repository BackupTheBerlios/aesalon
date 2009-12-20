#ifndef AESALON_INTERFACE_DWARF_ATTRIBUTE_DATA_H
#define AESALON_INTERFACE_DWARF_ATTRIBUTE_DATA_H

#include "Types.h"
#include "EntryID.h"

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
namespace DWARF {

class Entry;

class AttributeData {
public:
    enum data_type_e {
        INTEGER_DATA,
        STRING_DATA,
        ENTRY_DATA,
        UNDEFINED_DATA
    };
private:
    data_type_e data_type;
    Word integer_data;
    std::string string_data;
    EntryID entry_id;
public:
    AttributeData() : data_type(UNDEFINED_DATA) {}
    virtual ~AttributeData() {}
    
    data_type_e get_data_type() const { return data_type; }
    void set_data_type(data_type_e new_data_type) { data_type = new_data_type; }
    
    Word get_integer() const { return integer_data; }
    std::string get_string() const { return string_data; }
    EntryID get_entry_id() const { return entry_id; }
};

} // namespace DWARF
} // namespace Interface
} // namespace Aesalon

#endif
