#ifndef AESALON_INTERFACE_GDB_PARSER_H
#define AESALON_INTERFACE_GDB_PARSER_H

#include <string>

namespace Aesalon {
namespace Interface {
namespace GDB {

class Parser {
public:
    Parser();
    virtual ~Parser();
    
    void parse_gdb_string(std::string string);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
