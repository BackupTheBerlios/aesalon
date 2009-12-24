#include <iostream>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <cstring>
#include <iostream>

#include "Portal.h"

namespace Aesalon {
namespace Interface {
namespace PTrace {

Platform::MemoryAddress Portal::get_register(register_e which) const {
    struct user_regs_struct registers;
    if(ptrace(PTRACE_GETREGS, pid, NULL, &registers) == -1)
        throw PTraceException(Misc::StreamAsString() << "Couldn't get register values: " << strerror(errno));
    
    switch(which) {
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
        case RAX:
            std::cout << "Value of RAX requested; RAX is " << registers.rax << ", ORIG_RAX is " << registers.orig_rax << std::endl;
            return registers.orig_rax;
        case RBX:
            return registers.rbx;
        case RIP:
            return registers.rip;
#endif
        default:
            throw PTraceException("Value of invalid register requested");
    }
}

Word Portal::read_memory(Platform::MemoryAddress address) const {
    SWord return_value = ptrace(PTRACE_PEEKDATA, pid, address, NULL);
    if(return_value == -1 && errno != 0)
        throw PTraceException(Misc::StreamAsString() << "Couldn't read memory: " << strerror(errno));
    /* NOTE: what happens if return_value is < 0? . ..  */
    return return_value;
}

void Portal::write_memory(Platform::MemoryAddress address, Word value) {
    if(ptrace(PTRACE_POKEDATA, pid, address, value) == -1) 
        throw PTraceException(Misc::StreamAsString() << "Couldn't write memory: " << strerror(errno));
}

void Portal::write_memory(Platform::MemoryAddress address, Byte value) {
    Word current_value = read_memory(address);
    current_value &= ~0xff;
    write_memory(address, current_value | value);
}

void Portal::attach() {
    ptrace(PTRACE_ATTACH, pid, NULL, NULL);
}

void Portal::place_breakpoint(Platform::MemoryAddress address) {
}

void Portal::handle_signal() {
    int signal;
    siginfo_t signal_info;
    if(ptrace(PTRACE_GETSIGINFO, pid, NULL, &signal_info) == -1) {
        throw PTraceException(Misc::StreamAsString() << "Failed to get signal information: " << strerror(errno));
    }
    signal = signal_info.si_signo;
    for(signal_observer_list_t::iterator i = signal_observer_list.begin(); i != signal_observer_list.end(); i ++) {
        if((*i)->handle_signal(signal)) return;
    }
}

void Portal::continue_execution(int signal) {
}

void Portal::single_step() {
    if(ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) == -1)
        throw PTraceException(Misc::StreamAsString() << "Couldn't single-step program:" << strerror(errno));
}

void Portal::wait_for_signal() {
}

} // namespace PTrace
} // namespace Interface
} // namespace Aesalon
