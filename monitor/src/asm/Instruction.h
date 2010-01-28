#ifndef AESALON_MONITOR_ASM_INSTRUCTION_H
#define AESALON_MONITOR_ASM_INSTRUCTION_H

#include "Types.h"
#include "Operand.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

class Instruction {
private:
    bool memory_change;
    Misc::SmartPointer<Operand> source;
    Misc::SmartPointer<Operand> destination;
    
    Word address;
    
    void handle_opcode(std::string opcode, std::string operands);
public:
    Instruction(std::string instruction, Word address);
    virtual ~Instruction() {}
    
    bool changes_memory() const { return memory_change; }
    
    Misc::SmartPointer<Operand> get_source() const { return source; }
    Misc::SmartPointer<Operand> get_destination() const { return destination; }
    
    Word get_address() const { return address; }
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
