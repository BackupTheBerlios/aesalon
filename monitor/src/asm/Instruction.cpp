/** Aesalon, a project to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009-2010 strange <kawk256@gmail.com>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    @file asm/Instruction.cpp
*/

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
        destination = storage_manager->new_operand(ASM::Operand::REGISTER, 0,
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
            Register::RAX
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
            Register::EAX
#endif
            )->get_storage_offset();
    }
}

} // namespace ASM
