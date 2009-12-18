#include <sys/user.h>
#include <sys/ptrace.h>

#include "PTracePortal.h"

namespace Aesalon {
namespace Interface {

Platform::MemoryAddress PTracePortal::get_register(register_e which) const {
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, 0, &regs);
    switch(which) {
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
        case RAX:
            return regs.rax;
        case RBX:
            return regs.rbx;
        case RCX:
            return regs.rcx;
        case RDX:
            return regs.rdx;
        case R9:
            return regs.r9;
        case R10:
            return regs.r10;
        case R11:
            return regs.r11;
        case R12:
            return regs.r12;
        case R13:
            return regs.r13;
        case R14:
            return regs.r14;
        case R15:
            return regs.r15;
        case RBP:
            return regs.rbp;
        case RSP:
            return regs.rsp;
        case RIP:
            return regs.rip;
#endif
        default:
            return 0;
    }
}

void PTracePortal::attach() {
    ptrace(PTRACE_ATTACH, pid, NULL, NULL);
}

} // namespace Interface
} // namespace Aesalon
