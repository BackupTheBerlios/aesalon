#include "Opcode.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
void Opcode::parse(Misc::SmartPointer<Block> block) {
    
}
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
    #error x86 Opcode disassembler currently NYI
#endif

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
