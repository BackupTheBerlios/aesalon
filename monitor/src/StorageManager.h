#ifndef AESALON_MONITOR_STORAGE_MANAGER_H
#define AESALON_MONITOR_STORAGE_MANAGER_H

#include <string>

#include "StorageOffset.h"
#include "asm/Register.h"
#include "asm/Operand.h"

namespace ASM {
class Instruction;
class InstructionList;
} // namespace ASM

class StorageManager {
public:
    enum data_type_e {
        OPERAND_DATA,
        INSTRUCTION_DATA,
        INSTRUCTION_LIST_DATA,
        BREAKPOINT_DATA
    };
private:
    Byte *data;
    StorageOffset data_size;
    StorageOffset unused_data_offset;
    void alloc_more();
public:
    StorageManager();
    ~StorageManager();
    
    ASM::Operand *new_operand(std::string operand_string);
    ASM::Operand *new_operand(ASM::Operand::operand_type_e type, Word address = 0, ASM::Register reg = ASM::Register::INVALID, SWord offset = 0);
};

#endif
