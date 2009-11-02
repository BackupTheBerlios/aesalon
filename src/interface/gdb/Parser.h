#ifndef AESALON_INTERFACE_GDB_PARSER_H
#define AESALON_INTERFACE_GDB_PARSER_H

#include <string>
#include "ParseData.h"
#include "String.h"

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class Parser {
private:
    std::string string;
    std::string::size_type begin;
    
    Misc::SmartPointer<ParseSequence> parse_data_sequence();
public:
    Parser();
    virtual ~Parser();
    
    Misc::SmartPointer<String> parse_gdb_string(std::string string);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
