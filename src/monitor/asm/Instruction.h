#ifndef AESALON_MONITOR_ASM_INSTRUCTION_H
#define AESALON_MONITOR_ASM_INSTRUCTION_H

#include "Types.h"
#include "platform/MemoryAddress.h"
#include "Opcode.h"
#include "Operand.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

class Instruction {
private:
    Platform::MemoryAddress address;
    Misc::SmartPointer<Block> raw;
    
    Opcode opcode;
    std::vector<Operand> operands;
    
    void parse_opcode();
public:
    Instruction(Platform::MemoryAddress address, Misc::SmartPointer<Block> raw);
    virtual ~Instruction() {}
    
    Platform::MemoryAddress get_address() const { return address; }
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
