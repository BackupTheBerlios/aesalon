#include <iostream>
#include "Instruction.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

Instruction::Instruction(Misc::SmartPointer<Block> block) {
    opcode = new Opcode(block);
    opcode->parse();
    
    for(std::vector<OperandType>::const_iterator i = opcode->get_expected_operands().begin(); i != opcode->get_expected_operands().end(); i ++) {
        operands.push_back(new Operand(*i, block));
    }
}

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
