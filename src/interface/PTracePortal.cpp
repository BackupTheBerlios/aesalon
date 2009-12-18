#include <sys/user.h>
#include <sys/ptrace.h>
#include <errno.h>
#include <signal.h>

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
        case RDI:
            return regs.rdi;
        case RSI:
            return regs.rsi;
        case RBP:
            return regs.rbp;
        case RSP:
            return regs.rsp;
        case RIP:
            return regs.rip;
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
        case EAX:
            return regs.eax;
        /* TODO: implement the rest of the 32-bit registers */
#endif
        case CS:
            return regs.cs;
        case SS:
            return regs.ss;
        default:
            return 0;
    }
}

Word PTracePortal::read_memory(Platform::MemoryAddress address) const {
    Word ret = ptrace(PTRACE_PEEKDATA, pid, address, NULL);
    if(errno) return 0;
    return ret;
}

void PTracePortal::write_memory(Platform::MemoryAddress address, Word value) {
    ptrace(PTRACE_POKEDATA, pid, address, value);
}

void PTracePortal::write_memory(Platform::MemoryAddress address, Byte value) {
    Word original = read_memory(address);
    /* Clear the first eight bits of original */
    original &= ~Word(0xff);
    /* Set first eight bits to new value */
    original |= value;
    ptrace(PTRACE_POKEDATA, pid, address, value);
}

void PTracePortal::attach() {
    ptrace(PTRACE_ATTACH, pid, NULL, NULL);
}

void PTracePortal::place_breakpoint(Platform::MemoryAddress address) {
    add_breakpoint(new Breakpoint(address, read_memory(address) & 0xff));
    write_memory(address, Byte(0xcc));
}

Misc::SmartPointer<Breakpoint> PTracePortal::get_breakpoint_by_address(Platform::MemoryAddress address) const {
    breakpoint_list_t::const_iterator i = breakpoint_list.begin();
    for(; i != breakpoint_list.end(); i ++) {
        if((*i)->get_address() == address) return *i;
    }
    return NULL;
}

int PTracePortal::handle_signal() {
    siginfo_t signal_info;
    
    ptrace(PTRACE_GETSIGINFO, pid, NULL, &signal_info);
    
    if(signal_info.si_signo != SIGSTOP) return signal_info.si_signo;
    
    Platform::MemoryAddress rip = get_register(RIP);
    Misc::SmartPointer<Breakpoint> breakpoint = get_breakpoint_by_address(rip);
    if(!breakpoint.is_valid()) return signal_info.si_signo;
    write_memory(rip, breakpoint->get_original());
    single_step();
    write_memory(rip, Byte(0xcc));
    
    return signal_info.si_signo;
}

void PTracePortal::continue_execution(int signal) {
    ptrace(PTRACE_CONT, pid, NULL, signal);
}

void PTracePortal::single_step() {
    ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
}

} // namespace Interface
} // namespace Aesalon
