#include <map>
#include <iostream>
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

Register Register::from_string(std::string string) {
    std::map<std::string, register_e> reg_map;
    
    reg_map["al"] = AL;
    reg_map["ah"] = AH;
    reg_map["bl"] = BL;
    reg_map["bh"] = BH;
    reg_map["cl"] = CL;
    reg_map["ch"] = CH;
    reg_map["dl"] = DL;
    reg_map["dh"] = DH;
    reg_map["ax"] = AX;
    reg_map["bx"] = BX;
    reg_map["cx"] = CX;
    reg_map["dx"] = DX;
    reg_map["eax"] = EAX;
    reg_map["ebx"] = EBX;
    reg_map["ecx"] = ECX;
    reg_map["edx"] = EDX;
    reg_map["esi"] = ESI;
    reg_map["edi"] = EDI;
    reg_map["esp"] = ESP;
    reg_map["ebp"] = EBP;
    reg_map["eip"] = EIP;
    reg_map["rax"] = RAX;
    reg_map["rbx"] = RBX;
    reg_map["rcx"] = RCX;
    reg_map["rdx"] = RDX;
    reg_map["r8"] = R8;
    reg_map["r9"] = R9;
    reg_map["r10"] = R10;
    reg_map["r11"] = R11;
    reg_map["r12"] = R12;
    reg_map["r13"] = R13;
    reg_map["r14"] = R14;
    reg_map["r15"] = R15;
    reg_map["rsi"] = RSI;
    reg_map["rdi"] = RDI;
    reg_map["rsp"] = RSP;
    reg_map["rbp"] = RBP;
    reg_map["rip"] = RIP;
    reg_map["cs"] = CS;
    reg_map["ss"] = SS;
    
    for(std::string::iterator i = string.begin(); i != string.end(); i ++) {
        (*i) = std::tolower(*i);
    }
    
    std::cout << "Register::from_string(): resulting register string is \"" << string << "\"\n";
    
    return reg_map[string];
}

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon
