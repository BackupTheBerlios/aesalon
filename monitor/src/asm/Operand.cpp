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
    
    @file asm/Operand.cpp
*/

#include <iostream>
#include "Operand.h"
#include "misc/String.h"

namespace ASM {

Operand::Operand(StorageOffset storage_offset, std::string operand_string) : storage_offset(storage_offset) {
    Register reg = Register::from_string(operand_string);
    if(reg != Register::INVALID) {
        operand_type = REGISTER;
        this->reg = reg;
        data_size = reg.get_register_size();
        return;
    }
    if(std::isdigit(operand_string[0])) {
        Misc::String::to<Word>(operand_string, address);
        operand_type = MEMORY_ADDRESS;
        return;
    }
    operand_type = REGISTER_OFFSET;
    /* Some examples:
        DWORD PTR [rbp-0x14]
        QWORD PTR [rip+0x200541]
    */
    if(Misc::String::begins_with(operand_string, "QWORD")) data_size = Register::SIZE_QWORD;
    else if(Misc::String::begins_with(operand_string, "DWORD")) data_size = Register::SIZE_DWORD;
    else if(Misc::String::begins_with(operand_string, "WORD")) data_size = Register::SIZE_WORD;
    else if(Misc::String::begins_with(operand_string, "BYTE")) data_size = Register::SIZE_BYTE;
    operand_string.erase(0, operand_string.find("[")+1);
    std::string offset;
    if(operand_string.find("+") != std::string::npos) {
        offset = operand_string.substr(operand_string.find("+"));
        operand_string.erase(operand_string.find("+"));
    }
    else if(operand_string.find("-") != std::string::npos) {
        offset = operand_string.substr(operand_string.find("-"));
        operand_string.erase(operand_string.find("-"));
    }
    Misc::String::to<SWord>(offset, this->offset);
    this->reg = Register::from_string(operand_string);
}

} // namespace ASM