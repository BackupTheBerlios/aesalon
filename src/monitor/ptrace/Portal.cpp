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
#include <fstream>
#include <sstream>
#include <climits>

#include "Portal.h"
#include "Initializer.h"

#include "ExitObserver.h"
#include "TrapObserver.h"
#include "SegfaultObserver.h"
#include "MallocObserver.h"
#include "MainObserver.h"
#include "BreakpointReference.h"

#include "misc/String.h"

#include "Message.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

Portal::Portal(Misc::SmartPointer<Platform::ArgumentList> argument_list) : pid(0), libc_offset(0) {
    std::cout << "Portal::Portal(): called, initialization begun . . . \n";
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
    
    /* Trap signals are the most common, so add the TrapObserver on first. */
    add_signal_observer(new TrapObserver());
    add_signal_observer(new ExitObserver());
    add_signal_observer(new SegfaultObserver());
    
    /* This is a single-shot breakpoint observer, but since there's currently no way to remove observers, stick it on last. */
    initial_observer = new MainObserver();
    malloc_observer = new MallocObserver();
    
    std::cout << "Portal::Portal(): waiting for SIGTRAP call from child . . ." << std::endl;
    /* Wait for the SIGTRAP that indicates a exec() call . . . */
    wait_for_signal();
    std::cout << "Portal::Portal(): got SIGTRAP call, placing breakpoint on main . . ." << std::endl;
    /* place a breakpoint at main, for intialization purposes. */
    Word main_address = Initializer::get_instance()->get_program_manager()->get_elf_parser()->get_symbol("main")->get_address();
    place_breakpoint(main_address, initial_observer);
    
    std::cout << "Portal::Portal(): continuing execution until breakpoint on main . . ." << std::endl;
    /* Now continue until main(). */
    continue_execution();
}

Word Portal::get_register(ASM::Register which) const {
    struct user_regs_struct registers;
    if(ptrace(PTRACE_GETREGS, pid, NULL, &registers) == -1)
        throw PTraceException(Misc::StreamAsString() << "Couldn't get register values: " << strerror(errno));
    
    switch(which) {
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
        case ASM::Register::RAX:
            /*std::cout << "Value of RAX requested; RAX is " << registers.rax << ", ORIG_RAX is " << registers.orig_rax << std::endl;*/
            return registers.rax;
        case ASM::Register::RBX:
            return registers.rbx;
        case ASM::Register::RDI:
            return registers.rdi;
        case ASM::Register::RIP:
            return registers.rip;
        case ASM::Register::RBP:
            return registers.rbp;
        case ASM::Register::RSP:
            return registers.rsp;
#endif
        default:
            throw PTraceException("Value of invalid register requested");
    }
}

void Portal::set_register(ASM::Register which, Word new_value) {
    struct user_regs_struct registers;
    std::cout << "Portal::set_register() called . . .\n";
    if(ptrace(PTRACE_GETREGS, pid, NULL, &registers) == -1)
        throw PTraceException(Misc::StreamAsString() << "Couldn't get register values: " << strerror(errno));
    
    switch(which) {
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
        case ASM::Register::RIP:
            std::cout << "\tSetting RIP to " << std::hex << new_value << std::dec << std::endl;
            registers.rip = new_value;
            break;
#endif
        default:
            throw PTraceException("Asked to set value of invalid register");
    }
    if(ptrace(PTRACE_SETREGS, pid, NULL, &registers) == -1)
        throw PTraceException(Misc::StreamAsString() << "Couldn't set register values: " << strerror(errno));
}

Word Portal::read_memory(Platform::MemoryAddress address) const {
    std::cout << "Portal::read_memory() called . . ." << std::endl;
    std::cout << "\tReading address " << std::hex << address << std::endl;
    Word return_value = ptrace(PTRACE_PEEKDATA, pid, address, NULL);
    if(return_value == Word(-1) && errno != 0)
        throw PTraceException(Misc::StreamAsString() << "Couldn't read memory: " << strerror(errno));
    return return_value;
}

void Portal::write_memory(Platform::MemoryAddress address, Word value) {
    std::cout << "Portal::write_memory(address, Word) called . . ." << std::endl;
    std::cout << "\tWriting " << std::hex <<value << " to " << address << std::endl;
    if(ptrace(PTRACE_POKEDATA, pid, address, value) == -1) 
        throw PTraceException(Misc::StreamAsString() << "Couldn't write memory: " << strerror(errno));
}

void Portal::write_memory(Platform::MemoryAddress address, Byte value) {
    std::cout << "Portal::write_memory(address, Byte) called . . ." << std::endl;
    std::cout << "\tWriting 0x" << std::hex << (int)value << " to " << address << std::endl;
    Word word_offset = 0;
    word_offset = address & 0x07;
    Word current_value = read_memory(address - word_offset);
    
    std::cout << "\tUtilizing write_memory(address, Word); word_offset is: " << word_offset << std::endl;
    std::cout << "\tBefore byte clearing, current_value is: " << current_value << std::endl;
    current_value &= ~(Word(0xff) << (word_offset * CHAR_BIT));
    std::cout << "\tAfter byte clearing, current_value is: " << current_value << std::endl;
    current_value |= (Word(value) << (word_offset * CHAR_BIT));
    std::cout << "\tAfter byte insertion, current_value is: " << current_value << std::endl;
    write_memory(address - word_offset, current_value);
}

void Portal::attach() {
    ptrace(PTRACE_ATTACH, pid, NULL, NULL);
}

void Portal::detach() {
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
}

std::size_t Portal::place_breakpoint(Platform::MemoryAddress address, Misc::SmartPointer<BreakpointObserver> observer) {
    std::cout << "Portal::place_breakpoint() called . . ." << std::endl;
    std::cout << "\tPlacing breakpoint at " << std::hex << address << std::dec << std::endl;
    Misc::SmartPointer<Breakpoint> bp = get_breakpoint_by_address(address);
    if(!bp.is_valid()) {
        Byte original = read_memory(address) & 0xff;
        bp = new Breakpoint(address, original);
        add_breakpoint(bp);
        write_memory(address, bp->get_breakpoint_character());
    }
    bp->add_observer(observer);
    return bp->get_id();
}

void Portal::remove_breakpoint(Platform::MemoryAddress address) {
    std::cout << "Portal::remove_breakpoint() called . . ." << std::endl;
    std::cout << "\tRemoving breakpoint at " << std::hex << address << std::dec << std::endl;
    
    Misc::SmartPointer<Breakpoint> bp = get_breakpoint_by_address(address);
    if(!bp.is_valid()) {
        std::cout << "\tAsked to remove non-existent breakpoint!" << std::endl;
        return;
    }
    Byte original = bp->get_original();
    write_memory(bp->get_address(), original);
    for(breakpoint_list_t::iterator i = breakpoint_list.begin(); i != breakpoint_list.end(); i ++) {
        if((*i)->get_id() == bp->get_id()) {
            breakpoint_list.erase(i);
            break;
        }
    }
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
    
    std::cout << std::dec;
    
    std::cout << "Portal::handle_signal(): status: (" << status << "): ";
    
    for(int mask = 1 << 20; mask > 0; mask >>= 1) {
        std::cout << ((status & mask)?"1":"0");
    }
    
    std::cout << ", signal: " << signal << std::endl;
    
    for(signal_observer_list_t::iterator i = signal_observer_list.begin(); i != signal_observer_list.end(); i ++) {
        if((*i)->handle_signal(signal, status)) return;
    }
    std::cout << "\twarning: unhandled signal!" << std::endl;
}

void Portal::continue_execution(int signal) {
    std::cout << "Portal::continue_execution() called . . ." << std::endl;
    if(ptrace(PTRACE_CONT, pid, NULL, NULL) == -1)
        throw PTraceException(Misc::StreamAsString() << "Couldn't continue program execution: " << strerror(errno));
    std::cout << "\tExecution continued." << std::endl;
}

void Portal::single_step() {
    std::cout << "Portal::single_step() called . . ." << std::endl;
    if(ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) == -1)
        throw PTraceException(Misc::StreamAsString() << "Couldn't single-step program:" << strerror(errno));
    wait_for_signal(); /* ptrace(PTRACE_SINGLESTEP throws a SIGTRAP when it's done, so wait for it. */
    std::cout << "\tSingle-step successful. " << std::endl;
}

int Portal::wait_for_signal() {
    std::cout << "Portal::wait_for_signal() called . . ." << std::endl;
    int status;
    if(waitpid(pid, &status, 0) == -1) throw PTraceException(Misc::StreamAsString() << "Couldn't waitpid() on child: " << strerror(errno));
    std::cout << "\tGot signal." << std::endl;
    return status;
}

void Portal::handle_breakpoint() {
    std::cout << "Portal::handle_breakpoint() called . . ." << std::endl;
    Word ip = get_register(
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    ASM::Register::RIP
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
    ASM::Register::EIP
#endif
    );
    /* Subtract one from the IP, since the 0xcc SIGTRAP instruction was executed . . . */
    ip --;
    Misc::SmartPointer<Breakpoint> breakpoint = get_breakpoint_by_address(ip);
    std::cout << "\tIP: " << std::hex << ip << std::dec << std::endl;
    if(!breakpoint.is_valid()) {
        /*Message(Message::DEBUG_MESSAGE, "handle_breakpoint() called on non-breakpoint");*/
        return;
    }
    Message(Message::DEBUG_MESSAGE, "handle_breakpoint() found a breakpoint . . .");
    
    /* ip is currently ($rip - 1), to use gdb notation. In other words, back up one byte. */
    set_register(ASM::Register::RIP, ip);
    
    breakpoint->notify();
    
    /* Write out the original instruction . . . */
    write_memory(breakpoint->get_address(), breakpoint->get_original());
    /* Single-step over that instruction . . . */
    single_step();
    /* Only re-write out the trap instruction if the breakpoint is still valid. */
    if(breakpoint->is_valid()) {
        /* Re-write out the trap instruction . . . */
        write_memory(breakpoint->get_address(), breakpoint->get_breakpoint_character());
    }
    else {
        remove_breakpoint(breakpoint->get_address());
    }
    /* And then TrapObserver will call continue_execution(). */
}

Word Portal::get_libc_offset() {
    if(libc_offset != 0) return libc_offset;
    libc_offset = 0;
    
    std::string map_file;
    map_file = Misc::StreamAsString() << "/proc/" << pid << "/maps";
    std::ifstream map_stream(map_file.c_str());
    if(!map_stream.is_open()) throw PTraceException(Misc::StreamAsString() << "Couldn't open " << map_file << ", perhaps permissions are screwy?");
    
    /*std::cout << "Memory map: " << std::endl;*/
    
    /* Example memory map excerpt (from bash):
        00400000-004d5000 r-xp 00000000 08:06 71555                              /bin/bash
        006d4000-006de000 rw-p 000d4000 08:06 71555                              /bin/bash
        006de000-006e3000 rw-p 00000000 00:00 0                                           
        015ba000-01a44000 rw-p 00000000 00:00 0                                  [heap]   
        7f7698328000-7f769832a000 r-xp 00000000 08:06 218888                     /usr/lib/gconv/ISO8859-1.so
        7f769832a000-7f7698529000 ---p 00002000 08:06 218888                     /usr/lib/gconv/ISO8859-1.so
        7f7698529000-7f769852a000 r--p 00001000 08:06 218888                     /usr/lib/gconv/ISO8859-1.so
        7f769852a000-7f769852b000 rw-p 00002000 08:06 218888                     /usr/lib/gconv/ISO8859-1.so
    */
    
    /* Get one line at a time, for EOF detection purposes. */
    
    char buffer[1024];
    while(!map_stream.eof() && map_stream.getline(buffer, 1024, '\n') && std::strlen(buffer)) {
        Word from, to;
        char spacer;
        Word offset;
        std::string mode, device, inode, path;
        
        std::stringstream line_stream;
        line_stream << buffer;
        line_stream >> std::hex >> from;
        line_stream >> spacer;
        line_stream >> std::hex >> to;
        line_stream >> mode;
        line_stream >> std::hex >> offset;
        line_stream >> device;
        line_stream >> inode;
        line_stream >> path;
        /*std::cout << "\tLibrary path is \"" << path << "\"\n";
        std::cout << "\tAddress range is: 0x" << std::hex << from << " to 0x" << to << " (size 0x" << to-from << ")" << std::endl;
        std::cout << "\tMap mode is: \"" << mode << "\"\n";
        std::cout << std::dec << std::endl;*/
        if(Misc::String::begins_with(path, "/lib/libc")) {
            if(mode == "r-xp") libc_offset = from;
        }
    }
    
    map_stream.close();
    
    return libc_offset;
}

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon
