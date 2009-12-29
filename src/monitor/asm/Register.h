#ifndef AESALON_MONITOR_ASM_REGISTER_H
#define AESALON_MONITOR_ASM_REGISTER_H

namespace Aesalon {
namespace Monitor {
namespace ASM {

class Register {
public:
    enum register_e {
#if AESALON_PLATFORM == AESALON_PLATFORM_x86
        EAX,
        EBX,
        ECX,
        EDX,
        ESI,
        EDI,
        EIP
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86_64
        RAX,
        RBX,
        RCX,
        RDX,
        R9,
        R10,
        R11,
        R12,
        R13,
        R14,
        R15,
        RSI,
        RDI,
        RIP
#endif
    };
private:
    register_e reg;
public:
    Register(register_e reg) : reg(reg) {}
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
};

} // namespace ASM
} // namespace Monitor
} // namespace Aesalon

#endif
