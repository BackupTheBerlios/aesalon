#ifndef AESALON_MONITOR_ASM_OPCODE_H
#define AESALON_MONITOR_ASM_OPCODE_H

#include "Types.h"
#include "Operand.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

class Opcode {
private:
    Misc::SmartPointer<Block> opcode;
    std::vector<OperandType> expected_operands;
public:
    Opcode(Misc::SmartPointer<Block> block);
    virtual ~Opcode() {}
    
    void parse();
    
    std::vector<OperandType> get_expected_operands() const { return expected_operands; }
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
