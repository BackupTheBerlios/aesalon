#ifndef AESALON_MONITOR_DWARF_ATTRIBUTE_H
#define AESALON_MONITOR_DWARF_ATTRIBUTE_H

#include <string>

#include "EntryID.h"
#include "Types.h"

#include "misc/SmartPointer.h"
#include "misc/Exception.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

class TypeMismatchException : public Misc::Exception {
public:
    TypeMismatchException() : Misc::Exception("Mismatch between data_type_e value and actual data type.") {}
    virtual ~TypeMismatchException() {}
};

class Attribute {
public:
    /* NOTE: this is the order the types appear in the DWARF 3 specification. */
    enum data_type_e {
        ADDRESS_DATA,
        BLOCK_DATA,
        CONSTANT_DATA,
        FLAG_DATA,
        LINE_POINTER_DATA,
        LOCATION_LIST_POINTER_DATA,
        MACRO_DEFINITION_POINTER_DATA,
        RANGE_LIST_POINTER_DATA,
        ENTRY_REFERENCE_DATA,
        STRING_DATA,
        UNINITIALIZED_DATA
    };
private:
    data_type_e data_type;
    struct {
        Word word_data;
        Block block_data;
        std::string string_data;
        bool flag_data;
    } internal_data;
public:
    Attribute() : data_type(UNINITIALIZED_DATA) {}
    virtual ~Attribute() {}
    
    void set_data(data_type_e new_type, Word word_data);
    void set_data(data_type_e new_type, Block block_data);
    void set_data(data_type_e new_type, std::string string_data);
    void set_data(data_type_e new_type, bool flag_data);
    
    Word as_word() const;
    Block as_block() const;
    std::string as_string() const;
    bool as_flag() const;
    
    data_type_e get_data_type() const { return data_type; }
    void set_data_type(data_type_e new_type) { data_type = new_type; }
};

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon

#endif
