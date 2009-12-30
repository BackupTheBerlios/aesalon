#include "Register.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

std::size_t Register::get_register_size() const {
#if AESALON_PLATFORM == AESALON_PLATFORM_x86 || AESALON_PLATFORM_x86_64
    switch(reg) {
        case AL:
        case AH:
        case BL:
        case BH:
        case CL:
        case CH:
        case DL:
        case DH:
            return 8;
        case AX:
        case BX:
        case CX:
        case DX:
            return 16;
        case EAX:
        case EBX:
        case ECX:
        case EDX:
        case EDI:
        case ESI:
        case EBP:
        case ESP:
            return 32;
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
        case RAX:
        case RBX:
        case RCX:
        case RDX:
        case R8:
        case R9:
        case R10:
        case R11:
        case R12:
        case R13:
        case R14:
        case R15:
        case RSI:
        case RDI:
        case RSP:
        case RBP:
        case RIP:
            return 64;
        default:
            return 0;
#endif
    }
#endif
    return 0;
}

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
