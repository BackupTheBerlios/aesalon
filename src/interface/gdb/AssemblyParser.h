#ifndef AESALON_INTERFACE_GDB_ASSEMBLY_PARSER_H
#define AESALON_INTERFACE_GDB_ASSEMBLY_PARSER_H

#include <string>

namespace Aesalon {
namespace Interface {
namespace GDB {

class AssemblyParser {
public:
    AssemblyParser();
    virtual ~AssemblyParser();
    
    bool changes_memory(std::string asm_line);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
