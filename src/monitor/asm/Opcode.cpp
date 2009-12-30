#include "Opcode.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

Opcode::Opcode(Misc::SmartPointer<Block> block) {
    std::size_t opcode_size = 0;
#if AESALON_PLATFORM == AESALON_PLATFORM_x86 || AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    if(*block->get_data(0) != 0x0f) opcode_size = 1;
    else opcode_size = 2;
#endif
    opcode = block->subset(0, opcode_size);
    block->remove(0, opcode_size);
}

void Opcode::parse() {
    expected_operands.push_back(OperandType(Aesalon::Monitor::ASM::OperandType::REGISTER, 8));
}

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
