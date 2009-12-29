#include <iostream>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <cstring>
#include <iostream>

#include "Portal.h"

#include "ExitObserver.h"
#include "TrapObserver.h"

#include "Message.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

Portal::Portal(Misc::SmartPointer<Platform::ArgumentList> argument_list) : pid(0) {
    
    pid = fork();
    if(pid == -1)
        throw PTraceException(Misc::StreamAsString() << "Forking to create child process failed: " << strerror(errno));
    else if(pid == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        if(execv(argument_list->get_argument(0).c_str(), argument_list->get_as_argv()) == -1) {
            throw PTraceException(Misc::StreamAsString() << "Failed to execute process: " << strerror(errno));
        }
    }
    
    /*ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);*/
    
    add_signal_observer(new ExitObserver());
    add_signal_observer(new TrapObserver());
}

Platform::MemoryAddress Portal::get_register(ASM::Register which) const {
    struct user_regs_struct registers;
    if(ptrace(PTRACE_GETREGS, pid, NULL, &registers) == -1)
        throw PTraceException(Misc::StreamAsString() << "Couldn't get register values: " << strerror(errno));
    
    switch(which) {
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
        case ASM::Register::RAX:
            std::cout << "Value of RAX requested; RAX is " << registers.rax << ", ORIG_RAX is " << registers.orig_rax << std::endl;
            return registers.orig_rax;
        case ASM::Register::RBX:
            return registers.rbx;
        case ASM::Register::RIP:
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
    Byte original = read_memory(address) & 0xff;
    add_breakpoint(new Breakpoint(address, original));
    write_memory(address, Byte(0xcc));
}

Misc::SmartPointer<Breakpoint> Portal::get_breakpoint_by_id(std::size_t which) const {
    for(breakpoint_list_t::const_iterator i = breakpoint_list.begin(); i != breakpoint_list.end(); i ++) {
        if((*i)->get_id() == which) return *i;
    }
    return NULL;
}

Misc::SmartPointer<Breakpoint> Portal::get_breakpoint_by_address(Platform::MemoryAddress address) const {
    for(breakpoint_list_t::const_iterator i = breakpoint_list.begin(); i != breakpoint_list.end(); i ++) {
        if((*i)->get_address() == address) return *i;
    }
    return NULL;
}


void Portal::handle_signal() {
    int signal;
    int status = wait_for_signal();
    if(!WIFEXITED(status) && WIFSTOPPED(status)) {
        siginfo_t signal_info;
        if(ptrace(PTRACE_GETSIGINFO, pid, NULL, &signal_info) == -1) {
            throw PTraceException(Misc::StreamAsString() << "Failed to get signal information: " << strerror(errno));
        }
        signal = signal_info.si_signo;
    }
    else signal = -1;
    
    std::cout << "Portal::handle_signal(): status: (" << status << "): ";
    
    for(int mask = 1 << 20; mask > 0; mask >>= 1) {
        std::cout << ((status & mask)?"1":"0");
    }
    
    std::cout << ", signal: " << signal << std::endl;
    
    for(signal_observer_list_t::iterator i = signal_observer_list.begin(); i != signal_observer_list.end(); i ++) {
        if((*i)->handle_signal(signal, status)) return;
    }
}

void Portal::continue_execution(int signal) {
    if(ptrace(PTRACE_CONT, pid, NULL, NULL) == -1)
        throw PTraceException(Misc::StreamAsString() << "Couldn't continue program execution: " << strerror(errno));
}

void Portal::single_step() {
    if(ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) == -1)
        throw PTraceException(Misc::StreamAsString() << "Couldn't single-step program:" << strerror(errno));
}

int Portal::wait_for_signal() {
    int status;
    if(waitpid(pid, &status, 0) == -1) throw PTraceException(Misc::StreamAsString() << "Couldn't waitpid() on child: " << strerror(errno));
    return status;
}

void Portal::handle_breakpoint() {
    Misc::SmartPointer<Breakpoint> breakpoint = get_breakpoint_by_address(get_register(
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    ASM::Register::RIP
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
    ASM::Register::EIP
#endif
    ));
    if(!breakpoint.is_valid()) {
        /*Message(Message::DEBUG_MESSAGE, "handle_breakpoint() called on non-breakpoint");*/
        return;
    }
    write_memory(breakpoint->get_address(), breakpoint->get_original());
    single_step();
    write_memory(breakpoint->get_address(), breakpoint->get_breakpoint_character());
}

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon
