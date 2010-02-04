#ifndef AESALON_MONITOR_ASM_INSTRUCTION_LIST_H
#define AESALON_MONITOR_ASM_INSTRUCTION_LIST_H

#include <vector>
#include "Types.h"
#include "Instruction.h"
#include "StorageOffset.h"

namespace ASM {

class InstructionList {
protected:
    typedef std::vector<StorageOffset> instruction_list_t;
private:
    StorageOffset storage_offset;
    Word offset;
    instruction_list_t instruction_list;
public:
    InstructionList(StorageOffset storage_offset, Word offset) : storage_offset(storage_offset), offset(offset) {}
    virtual ~InstructionList() {}
    
    StorageOffset get_storage_offset() const { return storage_offset; }
    
    void add_instruction(StorageOffset new_instruction)
        { instruction_list.push_back(new_instruction); }
    StorageOffset get_instruction(Word address) const {
        if(address > offset) return -1;
        return instruction_list[address-offset];
    }
    std::size_t get_instruction_count() const;
    StorageOffset get_instruction_by_index(std::size_t index) const;
    
};

} // namespace ASM

#endif
