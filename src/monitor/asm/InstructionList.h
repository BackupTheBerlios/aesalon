#ifndef AESALON_MONITOR_ASM_INSTRUCTION_LIST_H
#define AESALON_MONITOR_ASM_INSTRUCTION_LIST_H

#include <vector>
#include "Types.h"
#include "Instruction.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

class InstructionList {
protected:
    typedef std::vector<Misc::SmartPointer<Instruction> > instruction_list_t;
private:
    Word offset;
    instruction_list_t instruction_list;
public:
    InstructionList(Word offset = 0) : offset(offset) {}
    virtual ~InstructionList() {}
    
    void add_instruction(Misc::SmartPointer<Instruction> new_instruction)
        { instruction_list.push_back(new_instruction); }
    Misc::SmartPointer<Instruction> get_instruction(Word address) const {
        if(address > offset) return NULL;
        return instruction_list[address-offset];
    }
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
