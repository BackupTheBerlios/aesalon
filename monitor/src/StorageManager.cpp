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
    munmap(data, data_size);
}

void StorageManager::alloc_more() {
    /* Allocate another megabyte . . . */
    data = static_cast<Byte *>(mremap(data, data_size, data_size += 1048576, MREMAP_MAYMOVE));
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
    unused_data_offset += sizeof(ASM::Operand);
    type_map[ins->get_storage_offset()] = INSTRUCTION_DATA;
    return ins;
}

ASM::InstructionList *StorageManager::new_instruction_list(Word offset) {
    while(unsigned(data_size - unused_data_offset) < sizeof(ASM::InstructionList)) alloc_more();
    ASM::InstructionList *il = new(data + unused_data_offset) ASM::InstructionList(unused_data_offset, offset);
    unused_data_offset += sizeof(ASM::Operand);
    type_map[il->get_storage_offset()] = INSTRUCTION_LIST_DATA;
    return il;
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