#include <iostream>
#include "Instruction.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

Instruction::Instruction(std::string instruction) {
    std::string opcode;
    bool has_operands = false;
    if(instruction.find(" ") == std::string::npos) opcode = instruction;
    else {
        opcode = instruction.substr(0, instruction.find(" "));
        has_operands = true;
    }
    instruction.erase(0, opcode.length());
    
    std::cout << "Instruction opcode is: \"" << opcode << "\"\n";
    
}

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
