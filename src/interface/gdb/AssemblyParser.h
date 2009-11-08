#ifndef AESALON_INTERFACE_GDB_ASSEMBLY_PARSER_H
#define AESALON_INTERFACE_GDB_ASSEMBLY_PARSER_H

#include <string>
#include <vector>

namespace Aesalon {
namespace Interface {
namespace GDB {

class AssemblyParser {
private:
    std::string assembly;
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    std::string instruction;
    typedef std::vector<std::string> operand_vector_t;
    operand_vector_t operands;
    
    bool handle_mov_instruction();
#endif
public:
    AssemblyParser();
    virtual ~AssemblyParser();
    
    bool changes_memory(std::string asm_line);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
