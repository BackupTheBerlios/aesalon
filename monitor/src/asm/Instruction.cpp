#include <iostream>
#include "Instruction.h"
#include "misc/String.h"
#include "Initializer.h"

namespace ASM {

Instruction::Instruction(StorageOffset storage_offset, std::string instruction, Word address)
    : storage_offset(storage_offset), source(NULL), destination(NULL), address(address) {
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
}

void Instruction::handle_opcode(std::string opcode, std::string operands) {
    StorageManager *storage_manager = Initializer::get_instance()->get_storage_manager();
    std::vector<std::string> operand;
    if(operands.find("#") != std::string::npos) operands.erase(operands.find("#"));
    while(operands.length()) {
        operand.push_back(operands.substr(0, operands.find(",")));
        if(operands.find(",") != std::string::npos) operands.erase(0, operands.find(",")+1);
        else operands = "";
    }
    
    if(Misc::String::begins_with(opcode, "mov")) {
        source = storage_manager->new_operand(operand[0])->get_storage_offset();
        destination = storage_manager->new_operand(operand[1])->get_storage_offset();
    }
    else if(Misc::String::begins_with(opcode, "add")) {
        source = storage_manager->new_operand(operand[0])->get_storage_offset();
        destination = storage_manager->new_operand(ASM::Operand::REGISTER, 0, Register::RAX)->get_storage_offset();
    }
}

} // namespace ASM
