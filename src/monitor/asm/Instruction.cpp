#include "Instruction.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

Instruction::Instruction(Word address, Misc::SmartPointer<Block> raw)
    : address(address), raw(raw) {
    
    parse_opcode();
}

void Instruction::parse_opcode() {
    opcode.parse(raw);
}

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
