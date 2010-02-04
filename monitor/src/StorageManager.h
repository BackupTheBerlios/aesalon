#ifndef AESALON_MONITOR_STORAGE_MANAGER_H
#define AESALON_MONITOR_STORAGE_MANAGER_H

#include <string>
#include <map>

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
        INVALID_DATA,
        OPERAND_DATA,
        INSTRUCTION_DATA,
        INSTRUCTION_LIST_DATA,
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
    
    ASM::Operand *get_operand(StorageOffset storage_offset) const;
    ASM::Instruction *get_instruction(StorageOffset storage_offset) const;
    ASM::InstructionList *get_instruction_list(StorageOffset storage_offset) const;
};

#endif
