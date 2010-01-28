#ifndef AESALON_MONITOR_ASM_REGISTER_H
#define AESALON_MONITOR_ASM_REGISTER_H

#include <cstddef>
#include <vector>
#include <string>

namespace Aesalon {
namespace Monitor {
namespace ASM {

class Register {
public:
    enum register_e {
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
    
    enum data_size_e {
        SIZE_BYTE,
        SIZE_WORD,
        SIZE_DWORD,
        SIZE_QWORD
    };

private:
    register_e reg;
public:
    Register(register_e reg = INVALID) : reg(reg) {}
    virtual ~Register() {}
    
    operator register_e() const {
        return reg;
    }
    
    bool operator==(const Register &other) const {
        return other.reg == reg;
    }
    bool operator==(const register_e &other) const {
        return other == reg;
    }
    bool operator!=(const Register &other) const {
        return other.reg != reg;
    }
    bool operator!=(const register_e &other) const {
        return other != reg;
    }
    
    register_e operator=(const Register &other) {
        return (reg = other.reg);
    }
    data_size_e get_register_size() const;
    
    static Register from_string(std::string string);
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
