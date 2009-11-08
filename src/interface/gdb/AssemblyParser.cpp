#include "AssemblyParser.h"
#include <iostream>

namespace Aesalon {
namespace Interface {
namespace GDB {

AssemblyParser::AssemblyParser() {

}

AssemblyParser::~AssemblyParser() {

}

bool AssemblyParser::changes_memory(std::string asm_line) {
    std::cout << "AssemblyParser: asked if \"" << asm_line << "\" changes memory . . ." << std::endl;
    return false;
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
