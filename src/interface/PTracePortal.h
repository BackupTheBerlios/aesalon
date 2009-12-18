#ifndef AESALON_INTERFACE_PTRACE_PORTAL_H
#define AESALON_INTERFACE_PTRACE_PORTAL_H

#include <sys/types.h>
#include <sys/user.h>

#include "platform/MemoryAddress.h"

namespace Aesalon {
namespace Interface {

class PTracePortal {
public:
    enum register_e {
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
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
        RBP,
        RSP,
        RIP
#endif
    };
private:
    pid_t pid;
public:
    PTracePortal(pid_t pid) : pid(pid) {}
    virtual ~PTracePortal() {}
    
/*    void temp() {
        struct user_regs_struct regs;
        regs.
    }*/
    
    Platform::MemoryAddress get_register(register_e which) const;
    
    void attach();
};

} // namespace Interface
} // namespace Aesalon

#endif
