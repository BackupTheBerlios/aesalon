#ifndef AESALON_PTRACE_REGISTER_H
#define AESALON_PTRACE_REGISTER_H

namespace PTrace {

enum Register {
    INVALID = 0,
#if AESALON_PLATFORM == AESALON_PLATFORM_x86 || AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    AL,
    AH,
    BL,
    BH,
    CL,
    CH,
    DL,
    DH,
    AX,
    BX,
    CX,
    DX,
    EAX,
    EBX,
    ECX,
    EDX,
    ESI,
    EDI,
    ESP,
    EBP,
    EIP,
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    RAX,
    RBX,
    RCX,
    RDX,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15,
    RSI,
    RDI,
    RSP,
    RBP,
    RIP,
#endif
    CS,
    SS
#endif
};

#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
#define IP_REGISTER RIP
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
#define IP_REGISTER EIP
#endif

} // namespace PTrace

#endif
