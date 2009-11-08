#include "AssemblyParser.h"
#include <iostream>

#ifndef AESALON_PLATFORM
    #error "AESALON_PLATFORM undefined"
#endif

#if AESALON_PLATFORM == AESALON_PLATFORM_x86
    #error "x86 support is currently NYI"
#endif

namespace Aesalon {
namespace Interface {
namespace GDB {

AssemblyParser::AssemblyParser() {

}

AssemblyParser::~AssemblyParser() {

}

bool AssemblyParser::changes_memory(std::string asm_line) {
    assembly = asm_line;
#if AESALON_PLATFORM == AESALON_PLATFORM_x86
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    instruction = assembly.substr(0, assembly.find(" "));
    assembly.erase(0, instruction.length());
    /* Instructions that can change memory:
        - mov*
        - inc
        - dec
    */
    
    operands.clear();
    
    while(assembly.length() != 0) {
        /* Remove whitespace */
        while(assembly[0] == ' ' || assembly[0] == '\t') assembly.erase(0, 1);
        /* Separate operand */
        std::string oper = assembly.substr(0, assembly.find(","));
        assembly.erase(0, oper.length()+1);
        operands.push_back(oper);
    }
    
    /* Check if the instruction is a move instruction */
    if(instruction.find("mov") == 0) {
        return handle_mov_instruction();
    }
    
#endif
    return false;
}

#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64

bool AssemblyParser::handle_mov_instruction() {
    /* If the second operand begins with a '%', then the content of memory is moving into a register (mov   $0x20, %rax) */
    if(operands[1][0] == '%') return false;
    /* If the first operand begins with a '%', but the second does not,
        then the content of a register is moving into memory (mov   %rax, [%r9]).
        Excuse the mixing of intel and GNU assembly. */
    else if(operands[0][0] == '%') return true;
    else return true; /* This should never happen, but in case it does, assume it changes memory. */
}

#endif

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
