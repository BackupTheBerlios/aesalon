/** Aesalon, a project to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009-2010 strange <kawk256@gmail.com>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    @file ptrace/Portal.cpp
*/

#include <iostream>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <climits>

#include "Portal.h"
#include "Initializer.h"

#include "ExitObserver.h"
#include "TrapObserver.h"
#include "SegfaultObserver.h"

#include "misc/String.h"

#include "misc/Message.h"

namespace PTrace {

Portal::Portal(Misc::ArgumentList *argument_list) : pid(0) {
#ifdef USE_OVERLOAD
    int fds[2];
    if(pipe(fds) == -1)
        throw Exception::PTraceException(Misc::StreamAsString() << "Could not create pipe: " << strerror(errno));
    Word malloc_offset = Initializer::get_instance()->get_analyzer_interface()->get_file(
        Initializer::get_instance()->get_argument_parser()->get_argument("libc-path")->get_data())->get_symbol_address("malloc");
    Analyzer::File *file = Initializer::get_instance()->get_analyzer_interface()->get_file();
    
    Word bits = file->get_attribute("platform_bits");
    
    std::string overload_filename = Initializer::get_instance()->get_argument_parser()->get_argument("overload-path")->get_data();
    if(bits == 32) {
        overload_filename += "/aesalon_overload_32.so";
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
        throw Exception::PTraceException("Currently impossible to execute 32-bit programs when Aesalon is compiled for 64-bit platforms.");
#endif
    }
    else if(bits == 64) overload_filename += "/aesalon_overload_64.so";
    else {
        throw Exception::PTraceException("Platform is not 32- or 64-bits. This shouldn't happen!");
    }
#endif
    pid = fork();
    if(pid == -1)
        throw Exception::PTraceException(Misc::StreamAsString() << "Forking to create child process failed: " << strerror(errno));
    else if(pid == 0) {
#ifndef USE_OVERLOAD
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
#else
        char *current_preload = getenv("LD_PRELOAD");
        std::string preload_string;
        if(current_preload) preload_string = current_preload;
        preload_string += ":";
        preload_string += overload_filename;
        setenv("LD_PRELOAD", preload_string.c_str(), 1);
        
        setenv("aesalon_pipe_fd", (Misc::StreamAsString() << fds[1]).operator std::string().c_str(), 1);
        setenv("aesalon_malloc_offset", (Misc::StreamAsString() << std::hex << malloc_offset).operator std::string().c_str(), 1);
        setenv("aesalon_libc_path", Initializer::get_instance()->get_argument_parser()->get_argument("libc-path")->get_data().c_str(), 1);
        close(fds[0]);
        fcntl(fds[1], F_SETFL, fcntl(fds[1], F_GETFL) & ~O_NONBLOCK);
#endif
        if(execv(argument_list->get_argument(0).c_str(), argument_list->get_as_argv()) == -1) {
            throw Exception::PTraceException(Misc::StreamAsString() << "Failed to execute process: " << strerror(errno));
        }
    }
#ifdef USE_OVERLOAD
    fcntl(fds[0], F_SETFL, fcntl(fds[0], F_GETFL) & ~O_NONBLOCK);
    close(fds[1]);
    pipe_fd = fds[0];
#endif
    
    /* Trap signals are the most common, so add the TrapObserver on first. */
#ifndef USE_OVERLOAD
    add_signal_observer(new TrapObserver());
#endif
    add_signal_observer(new ExitObserver());
    add_signal_observer(new SegfaultObserver());
    
    map_parser = new MapParser(pid);
    
    /* The overload library will set itself up from here . . . */
}

Portal::~Portal() {
    delete map_parser;
    
    for(breakpoint_list_t::iterator i = breakpoint_list.begin(); i != breakpoint_list.end(); i ++) {
        delete *i;
    }
    for(signal_observer_list_t::iterator i = signal_observer_list.begin(); i != signal_observer_list.end(); i ++) {
        delete *i;
    }
}

Word Portal::get_register(Register which) const {
    struct user_regs_struct registers;
    if(ptrace(PTRACE_GETREGS, pid, NULL, &registers) == -1)
        throw Exception::PTraceException(Misc::StreamAsString() << "Couldn't get register values: " << strerror(errno));
    
    switch(which) {
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
        case RAX:
            /*std::cout << "Value of RAX requested; RAX is " << registers.rax << ", ORIG_RAX is " << registers.orig_rax << std::endl;*/
            return registers.rax;
        case RBX:
            return registers.rbx;
        case RDI:
            return registers.rdi;
        case RSI:
            return registers.rsi;
        case RIP:
            return registers.rip;
        case RBP:
            return registers.rbp;
        case RSP:
            return registers.rsp;
#endif
        default:
            throw Exception::PTraceException("Value of invalid register requested");
    }
}

void Portal::set_register(Register which, Word new_value) {
    struct user_regs_struct registers;
    if(ptrace(PTRACE_GETREGS, pid, NULL, &registers) == -1)
        throw Exception::PTraceException(Misc::StreamAsString() << "Couldn't set register values: " << strerror(errno));
    
    switch(which) {
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
        case RIP:
            registers.rip = new_value;
            break;
#endif
        default:
            throw Exception::PTraceException("Asked to set value of invalid register");
    }
    if(ptrace(PTRACE_SETREGS, pid, NULL, &registers) == -1)
        throw Exception::PTraceException(Misc::StreamAsString() << "Couldn't set register values: " << strerror(errno));
}

Word Portal::read_memory(Word address) const {
    Word return_value = ptrace(PTRACE_PEEKDATA, pid, address, NULL);
    if(return_value == Word(-1) && errno != 0)
        throw Exception::PTraceException(Misc::StreamAsString() << "Couldn't read memory: " << strerror(errno));
    return return_value;
}

void Portal::write_memory(Word address, Word value) {
    if(ptrace(PTRACE_POKEDATA, pid, address, value) == -1) 
        throw Exception::PTraceException(Misc::StreamAsString() << "Couldn't write memory: " << strerror(errno));
}

void Portal::write_memory(Word address, Byte value) {
    Word word_offset = 0;
    word_offset = address & 0x07;
    Word current_value = read_memory(address - word_offset);
    
    current_value &= ~(Word(0xff) << (word_offset * CHAR_BIT));
    current_value |= (Word(value) << (word_offset * CHAR_BIT));
    write_memory(address - word_offset, current_value);
}

void Portal::attach() {
    ptrace(PTRACE_ATTACH, pid, NULL, NULL);
}

void Portal::detach() {
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
}

std::size_t Portal::place_breakpoint(Word address, BreakpointObserver *observer) {
    Breakpoint *bp = get_breakpoint_by_address(address);
    if(!bp) {
        Byte original = read_memory(address) & 0xff;
        bp = new Breakpoint(address, original);
        add_breakpoint(bp);
        write_memory(address, bp->get_breakpoint_character());
    }
    bp->add_observer(observer);
    return bp->get_id();
}

void Portal::remove_breakpoint(Word address) {
    Breakpoint *bp = get_breakpoint_by_address(address);
    if(!bp) {
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
    delete bp;
}

Breakpoint *Portal::get_breakpoint_by_id(std::size_t which) const {
    for(breakpoint_list_t::const_iterator i = breakpoint_list.begin(); i != breakpoint_list.end(); i ++) {
        if((*i)->get_id() == which) return *i;
    }
    return NULL;
}

Breakpoint *Portal::get_breakpoint_by_address(Word address) const {
    Breakpoint * const *bp = &breakpoint_list[0];
    for(std::size_t x = 0; x < breakpoint_list.size(); x ++) {
        if(bp[x]->get_address() == address) return bp[x];
    }
    return NULL;
}

void Portal::handle_signal() {
    int status;
    if(wait_for_signal(status) == 0) return;
    int signal = 0;
    if(WIFSTOPPED(status)) {
        signal = WSTOPSIG(status);
    }
    else if(WIFSIGNALED(status)) signal = WTERMSIG(status);
    else if(WIFEXITED(status)) signal = -1;
    else return;
    
    for(signal_observer_list_t::iterator i = signal_observer_list.begin(); i != signal_observer_list.end(); i ++) {
        if((*i)->handle_signal(signal, status)) return;
    }
#ifndef USE_OVERLOAD
    /* If the signal wasn't caught by one of the signal handlers, then we're not interested in it. Continue execution. */
    continue_execution(signal);
#endif
}

void Portal::continue_execution(int signal) {
    if(ptrace(PTRACE_CONT, pid, NULL, NULL) == -1)
        throw Exception::PTraceException(Misc::StreamAsString() << "Couldn't continue program execution: " << strerror(errno));
}

void Portal::single_step() {
    if(ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) == -1)
        throw Exception::PTraceException(Misc::StreamAsString() << "Couldn't single-step program:" << strerror(errno));
    int status;
    wait_for_signal(status); /* ptrace(PTRACE_SINGLESTEP throws a SIGTRAP when it's done, so wait for it. */
}

int Portal::wait_for_signal(int &status) {
    int return_value;
    return_value = 
#ifndef USE_OVERLOAD
    waitpid(pid, &status, 0);
#else
    waitpid(pid, &status, WNOHANG);
#endif
    if(return_value == -1) throw Exception::PTraceException(Misc::StreamAsString() << "Couldn't waitpid() on child: " << strerror(errno));
    return return_value;
}

void Portal::handle_breakpoint() {
    Word ip = get_register(IP_REGISTER);
    /* Subtract one from the IP, since the 0xcc SIGTRAP instruction was executed . . . */
    ip --;
    Breakpoint *breakpoint = get_breakpoint_by_address(ip);
    if(!breakpoint) {
        /*Message(Message::DEBUG_MESSAGE, "handle_breakpoint() called on non-breakpoint");*/
        return;
    }
    if(breakpoint->get_original() != 0xcc) {
        /* ip is currently ($rip - 1), to use gdb notation. In other words, back up one byte. */
        
        set_register(IP_REGISTER, ip);
    }
    
    breakpoint->notify();
    
    if(breakpoint->get_original() != 0xcc) {
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
    }
    /* And then TrapObserver will call continue_execution(). */
}

Word Portal::get_lib_offset(std::string unique_identifer) {
    Word lib_offset = 0;
    std::string map_file;
    map_file = Misc::StreamAsString() << "/proc/" << pid << "/maps";
    std::ifstream map_stream(map_file.c_str());
    if(!map_stream.is_open()) throw Exception::PTraceException(Misc::StreamAsString() << "Couldn't open " << map_file << ", perhaps permissions are screwy?");
    
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
    while(!map_stream.eof() && map_stream.getline(buffer, 1024, '\n') && strlen(buffer)) {
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
        if(path.find(unique_identifer) != std::string::npos) {
            if(mode == "r-xp") lib_offset = from;
        }
    }
    
    map_stream.close();
    
    return lib_offset;
}

} // namespace PTrace
