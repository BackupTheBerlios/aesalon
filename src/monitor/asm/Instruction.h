#ifndef AESALON_MONITOR_ASM_INSTRUCTION_H
#define AESALON_MONITOR_ASM_INSTRUCTION_H

#include "Types.h"
#include "Opcode.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

class Instruction {
protected:
    typedef std::vector<Misc::SmartPointer<Operand> > operand_list_t;
private:
    operand_list_t operands;
public:
    Instruction(std::string instruction);
    virtual ~Instruction() {}
    
    Misc::SmartPointer<Operand> get_operand(std::size_t which) const
        { return operands[which]; }
    std::size_t get_operands() const { return operands.size(); }
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
