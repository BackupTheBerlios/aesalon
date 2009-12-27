#include "Instruction.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

Instruction::Instruction(Platform::MemoryAddress address, Misc::SmartPointer<Block> raw)
    : address(address), raw(raw) {
    
    parse_opcode();
}

void Instruction::parse_opcode() {
#if AESALON_PLATFORM == AESALON_PLATFORM_x86 || AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    if(*raw->get_data(0) == 0x0f) { /* it's a two-byte opcode */ }
#endif
}

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
