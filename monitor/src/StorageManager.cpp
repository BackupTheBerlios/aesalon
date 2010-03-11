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
    
    @file StorageManager.cpp
    This file contains the implementation of the StorageManager class.
*/

#include <sys/mman.h>

#include "StorageManager.h"
#include "asm/Operand.h"
#include "asm/Instruction.h"
#include "asm/InstructionList.h"

StorageManager::StorageManager() : data(NULL), data_size(0), unused_data_offset(0) {
    /* Start off with a megabyte of memory . . . */
    data_size = 1048576;
    data = static_cast<Byte *>(mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
}

StorageManager::~StorageManager() {
    for(type_map_t::iterator i = type_map.begin(); i != type_map.end(); i ++) {
        if(i->second == OPERAND_DATA) get_operand(i->first)->~Operand();
        else if(i->second == INSTRUCTION_DATA) get_instruction(i->first)->~Instruction();
        else if(i->second == INSTRUCTION_LIST_DATA) get_instruction_list(i->first)->~InstructionList();
    }
    munmap(data, data_size);
}

void StorageManager::alloc_more() {
    /* Allocate another megabyte . . . */
    data = static_cast<Byte *>(mremap(data, data_size, data_size + 1048576, MREMAP_MAYMOVE));
    data_size += 1048576;
}

ASM::Operand *StorageManager::new_operand(std::string operand_string) {
    while(unsigned(data_size - unused_data_offset) < sizeof(ASM::Operand)) alloc_more();
    ASM::Operand *operand = new(data + unused_data_offset) ASM::Operand(unused_data_offset, operand_string);
    unused_data_offset += sizeof(ASM::Operand);
    return operand;
}

ASM::Operand *StorageManager::new_operand(ASM::Operand::operand_type_e type, Word address, ASM::Register reg, SWord offset) {
    while(unsigned(data_size - unused_data_offset) < sizeof(ASM::Operand)) alloc_more();
    ASM::Operand *operand = new(data + unused_data_offset) ASM::Operand(unused_data_offset, type, address, reg, offset);
    unused_data_offset += sizeof(ASM::Operand);
    type_map[operand->get_storage_offset()] = OPERAND_DATA;
    return operand;
}

ASM::Instruction *StorageManager::new_instruction(std::string instruction, Word address) {
    while(unsigned(data_size - unused_data_offset) < sizeof(ASM::Instruction)) alloc_more();
    ASM::Instruction *ins = new(data + unused_data_offset) ASM::Instruction(unused_data_offset, instruction, address);
    unused_data_offset += sizeof(ASM::Instruction);
    type_map[ins->get_storage_offset()] = INSTRUCTION_DATA;
    return ins;
}

ASM::InstructionList *StorageManager::new_instruction_list(Word offset) {
    while(unsigned(data_size - unused_data_offset) < sizeof(ASM::InstructionList)) alloc_more();
    ASM::InstructionList *il = new(data + unused_data_offset) ASM::InstructionList(unused_data_offset, offset);
    unused_data_offset += sizeof(ASM::InstructionList);
    type_map[il->get_storage_offset()] = INSTRUCTION_LIST_DATA;
    return il;
}

ELF::Symbol *StorageManager::new_symbol(std::string symbol_name, Word address, Word size) {
    while(unsigned(data_size - unused_data_offset) < sizeof(ELF::Symbol)) alloc_more();
    ELF::Symbol *symbol = new(data + unused_data_offset) ELF::Symbol(unused_data_offset, symbol_name, address, size);
    unused_data_offset += sizeof(ELF::Symbol);
    type_map[symbol->get_storage_offset()] = SYMBOL_DATA;
    return symbol;
}

ASM::Operand *StorageManager::get_operand(StorageOffset storage_offset) const {
    if(type_map.at(storage_offset) != OPERAND_DATA) return 0;
    return reinterpret_cast<ASM::Operand *>(data + storage_offset);
}

ASM::Instruction *StorageManager::get_instruction(StorageOffset storage_offset) const {
    if(type_map.at(storage_offset) != INSTRUCTION_DATA) return 0;
    return reinterpret_cast<ASM::Instruction *>(data + storage_offset);
}

ASM::InstructionList *StorageManager::get_instruction_list(StorageOffset storage_offset) const {
    if(type_map.at(storage_offset) != INSTRUCTION_LIST_DATA) return 0;
    return reinterpret_cast<ASM::InstructionList *>(data + storage_offset);
}

ELF::Symbol *StorageManager::get_symbol(StorageOffset storage_offset) const {
    if(type_map.at(storage_offset) != SYMBOL_DATA) return 0;
    return reinterpret_cast<ELF::Symbol *>(data + storage_offset);
}
