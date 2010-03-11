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
    
    @file StorageManager.h
    This file contains the definition of the StorageManager class.
*/

#ifndef AESALON_MONITOR_STORAGE_MANAGER_H
#define AESALON_MONITOR_STORAGE_MANAGER_H

#include <string>
#include <map>

#include "StorageOffset.h"
#include "asm/Register.h"
#include "asm/Operand.h"
#include "elf/Symbol.h"

namespace ASM {
class Instruction;
class InstructionList;
} // namespace ASM

class StorageManager {
public:
    enum data_type_e {
        INVALID_DATA,
        OPERAND_DATA,
        INSTRUCTION_DATA,
        INSTRUCTION_LIST_DATA,
        SYMBOL_DATA,
        BREAKPOINT_DATA
    };
private:
    typedef std::map<StorageOffset, data_type_e> type_map_t;
    Byte *data;
    StorageOffset data_size;
    StorageOffset unused_data_offset;
    void alloc_more();
    type_map_t type_map;
public:
    StorageManager();
    ~StorageManager();
    
    ASM::Operand *new_operand(std::string operand_string);
    ASM::Operand *new_operand(ASM::Operand::operand_type_e type, Word address = 0, ASM::Register reg = ASM::Register::INVALID, SWord offset = 0);
    ASM::Instruction *new_instruction(std::string instruction, Word address);
    ASM::InstructionList *new_instruction_list(Word offset);
    ELF::Symbol *new_symbol(std::string symbol_name, Word address, Word size);
    
    ASM::Operand *get_operand(StorageOffset storage_offset) const;
    ASM::Instruction *get_instruction(StorageOffset storage_offset) const;
    ASM::InstructionList *get_instruction_list(StorageOffset storage_offset) const;
    ELF::Symbol *get_symbol(StorageOffset storage_offset) const;
};

#endif
