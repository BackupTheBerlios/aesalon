#include <iostream>
#include "Instruction.h"
#include "misc/String.h"

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
    
    std::cout << "\tInstruction opcode is: \"" << opcode << "\"\n";
    
    handle_opcode(opcode, Misc::String::strip_whitespace(instruction));
}


void Instruction::handle_opcode(std::string opcode, std::string operands) {
    if(opcode == "mov") {
        source = new Operand(operands.substr(0, operands.find(",")));
        destination = new Operand(operands.substr(operands.find(",")+1));
    }
}

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
