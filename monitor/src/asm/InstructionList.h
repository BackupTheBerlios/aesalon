#ifndef AESALON_MONITOR_ASM_INSTRUCTION_LIST_H
#define AESALON_MONITOR_ASM_INSTRUCTION_LIST_H

#include <vector>
#include "Types.h"
#include "Instruction.h"

namespace ASM {

class InstructionList {
protected:
    typedef std::vector<Instruction *> instruction_list_t;
private:
    Word offset;
    instruction_list_t instruction_list;
public:
    InstructionList(Word offset = 0) : offset(offset) {}
    virtual ~InstructionList();
    
    void add_instruction(Instruction *new_instruction)
        { instruction_list.push_back(new_instruction); }
    Instruction *get_instruction(Word address) const {
        if(address > offset) return NULL;
        return instruction_list[address-offset];
    }
    std::size_t get_instruction_count() const;
    Instruction *get_instruction_by_index(std::size_t index) const;
    
};

} // namespace ASM

#endif
