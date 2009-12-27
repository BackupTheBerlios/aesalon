#ifndef AESALON_MONITOR_ASM_INSTRUCTION_H
#define AESALON_MONITOR_ASM_INSTRUCTION_H

#include "Types.h"
#include "platform/MemoryAddress.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

class Instruction {
private:
    Platform::MemoryAddress address;
    Misc::SmartPointer<Block> raw;
    
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
