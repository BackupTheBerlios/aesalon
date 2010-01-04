#include "Register.h"

namespace Aesalon {
namespace Monitor {
namespace ASM {

Register::data_size_e Register::get_register_size() const {
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
            return SIZE_BYTE;
        case AX:
        case BX:
        case CX:
        case DX:
            return SIZE_WORD;
        case EAX:
        case EBX:
        case ECX:
        case EDX:
        case EDI:
        case ESI:
        case EBP:
        case ESP:
            return SIZE_DWORD;
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
            return SIZE_QWORD;
        default:
            return SIZE_BYTE;
#endif
    }
#endif
    return SIZE_BYTE;
}

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
