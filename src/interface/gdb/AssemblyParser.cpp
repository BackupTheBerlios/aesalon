#include "AssemblyParser.h"
#include <iostream>

#ifndef AESALON_PLATFORM
    #error "AESALON_PLATFORM undefined"
#endif

namespace Aesalon {
namespace Interface {
namespace GDB {

AssemblyParser::AssemblyParser() {

}

AssemblyParser::~AssemblyParser() {

}

bool AssemblyParser::changes_memory(std::string asm_line) {
    std::cout << "AssemblyParser: asked if \"" << asm_line << "\" changes memory . . ." << std::endl;
    
    std::string instruction = asm_line.substr(0, asm_line.find(" "));
    asm_line.erase(0, asm_line.length());
    std::cout << "\tAssembly instruction is " << instruction << std::endl;
    
#if AESALON_PLATFORM == AESALON_PLATFORM_x86
    #error "x86 support is currently NYI"
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    
    
    
#endif
    return false;
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
