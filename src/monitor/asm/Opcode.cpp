#include "Opcode.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
void Opcode::parse(Misc::SmartPointer<Block> block) {
    Byte first = *block->get_data(0);
    
    if(first == 0x0f) handle_two_byte();
    
    if(first <= 0x05) opcode = "ADD";
    else if(first >= 0x08 && first <= 0x0D) opcode = "OR";
    else if(first >= 0x10 && first <= 0x15) opcode = "ADC";
    else if(first >= 0x18 && first <= 0x1D) opcode = "SBB";
    else if(first >= 0x20 && first <= 0x25) opcode = "AND";
    else if(first >= 0x28 && first <= 0x2D) opcode = "SUB";
    else opcode = "<invalid opcode>";
}

void Opcode::handle_two_byte() {
    
}
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
    #error x86 Opcode disassembler currently NYI
#endif

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
