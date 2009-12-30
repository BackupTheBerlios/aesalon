#include <iostream>
#include "Instruction.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

Instruction::Instruction(Misc::SmartPointer<Block> block) {
    opcode = new Opcode(block);
}

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
