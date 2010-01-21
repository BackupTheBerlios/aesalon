#include "InstructionList.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

std::size_t InstructionList::get_instruction_count() const {
    std::size_t count = 0;
    for(instruction_list_t::const_iterator i = instruction_list.begin(); i != instruction_list.end(); i ++) {
        if((*i).is_valid()) count ++;
    }
    return count;
}

Misc::SmartPointer<Instruction> InstructionList::get_instruction_by_index(std::size_t index) const {
    instruction_list_t::const_iterator i = instruction_list.begin();
    for(; index > 0 && i != instruction_list.end(); i ++) {
        if((*i).is_valid()) index --;
    }
    if(i == instruction_list.end()) return NULL;
    return *i;
}

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
