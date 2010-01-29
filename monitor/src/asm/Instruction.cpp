#include <iostream>
#include "Instruction.h"
#include "misc/String.h"

namespace ASM {

Instruction::Instruction(std::string instruction, Word address) : source(NULL), destination(NULL), address(address) {
    std::string opcode;
    bool has_operands = false;
    if(instruction.find(" ") == std::string::npos) opcode = instruction;
    else {
        opcode = instruction.substr(0, instruction.find(" "));
        has_operands = true;
    }
    instruction.erase(0, opcode.length());
    
    /*std::cout << "\tInstruction opcode is: \"" << opcode << "\"\n";*/
    
    handle_opcode(opcode, Misc::String::strip_whitespace(instruction));
}

Instruction::~Instruction() {
    if(source != NULL) delete source;
    if(destination != NULL) delete destination;
}

void Instruction::handle_opcode(std::string opcode, std::string operands) {
    std::vector<std::string> operand;
    if(operands.find("#") != std::string::npos) operands.erase(operands.find("#"));
    while(operands.length()) {
        operand.push_back(operands.substr(0, operands.find(",")));
        if(operands.find(",") != std::string::npos) operands.erase(0, operands.find(",")+1);
        else operands = "";
    }
    
    if(Misc::String::begins_with(opcode, "mov")) {
        source = new Operand(operand[0]);
        destination = new Operand(operand[1]);
    }
    else if(Misc::String::begins_with(opcode, "add")) {
        source = new Operand(operand[0]);
        destination = new Operand(ASM::Operand::REGISTER, 0, Register::RAX);
    }
}

} // namespace ASM
