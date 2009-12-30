#ifndef AESALON_MONITOR_ASM_INSTRUCTION_H
#define AESALON_MONITOR_ASM_INSTRUCTION_H

#include "Types.h"
#include "Opcode.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

class Instruction {
private:
    Misc::SmartPointer<Opcode> opcode;
public:
    Instruction(Misc::SmartPointer<Block> block);
    virtual ~Instruction() {}
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
