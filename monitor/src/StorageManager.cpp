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
    while((data_size - unused_data_offset) < sizeof(ASM::Operand)) alloc_more();
    ASM::Operand *operand = new(data + unused_data_offset) ASM::Operand(unused_data_offset, operand_string);
    unused_data_offset += sizeof(ASM::Operand);
    return operand;
}

ASM::Operand *StorageManager::new_operand(ASM::Operand::operand_type_e type, Word address, ASM::Register reg, SWord offset) {
    while((data_size - unused_data_offset) < sizeof(ASM::Operand)) alloc_more();
    ASM::Operand *operand = new(data + unused_data_offset) ASM::Operand(unused_data_offset, type, address, reg, offset);
    unused_data_offset += sizeof(ASM::Operand);
    return operand;
}
