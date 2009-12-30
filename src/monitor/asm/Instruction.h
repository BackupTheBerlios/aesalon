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
    Misc::SmartPointer<Opcode> opcode;
    operand_list_t operands;
public:
    Instruction(Misc::SmartPointer<Block> block);
    virtual ~Instruction() {}
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
