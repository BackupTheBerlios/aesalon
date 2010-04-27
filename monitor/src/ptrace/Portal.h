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
    
    @file ptrace/Portal.h
*/

#ifndef AESALON_MONITOR_PTRACE_PORTAL_H
#define AESALON_MONITOR_PTRACE_PORTAL_H

#include <sys/types.h>
#include <sys/user.h>

#include <vector>

#include "Breakpoint.h"
#include "Types.h"
#include "SignalObserver.h"
#include "BreakpointObserver.h"
#include "misc/ArgumentList.h"
#include "exception/PTraceException.h"
#include "Register.h"
#include "MapParser.h"

namespace PTrace {

class Portal {
protected:
    typedef std::vector<Breakpoint *> breakpoint_list_t;
    
    typedef std::vector<SignalObserver *> signal_observer_list_t;
private:
    /** The PID of the attached process. */
    pid_t pid;
    
    /** The vector of breakpoints. */
    breakpoint_list_t breakpoint_list;
    
    signal_observer_list_t signal_observer_list;
        
    int wait_for_signal(int &status);

    MapParser *map_parser;
#ifdef USE_OVERLOAD
    int pipe_fd;
#endif
public:
    /** Generic constructor for PTracePortal.
        @param argument_list The arguments to spawn the child with.
    */
    Portal(Misc::ArgumentList *argument_list);
    /** Virtual destructor. */
    virtual ~Portal();
    
    /** Retrieve the value of a specific register.
        @param which The register to get the value of.
        @return The value of the specified register.
    */
    Word get_register(Register which) const;
    
    void set_register(Register which, Word new_value);
    
    /** Read a Word of memory.
        @param address The address to read.
        @return The value at @a address.
    */
    Word read_memory(Word address) const;
    /** Write a Word of memory.
        @param address The address to write to.
        @param value The value to write.
    */
    void write_memory(Word address, Word value);
    /** Write a Byte of memory.
        @param address The address to write to.
        @param value The value to write.
    */
    void write_memory(Word address, Byte value);
    
    /** Attaches onto the PID @a pid. */
    void attach();
    /** Detaches from running program. */
    void detach();
    
    /** Adds a breakpoint onto the list.
        @note Do not use this function speicially unless you know what you are doing.
        @param breakpoint The breakpoint to add.
    */
    void add_breakpoint(Breakpoint *breakpoint) {
        breakpoint_list.push_back(breakpoint);
    }
    
    /** Places a breakpoint at a specified address.
        @param address The address to place the breakpoint at.
        @param observer The initial observer to use for the breakpoint.
        @return The ID of the newly-inserted breakpoint.
    */
    std::size_t place_breakpoint(Word address, BreakpointObserver *observer);
    
    void remove_breakpoint(Word address);
    
    /** Returns a breakpoint by iterator.
        @param which The breakpoint to find.
        @return A SmartPointer to the given breakpoint. Not valid if the breakpoint iterator does not exist.
    */
    Breakpoint *get_breakpoint(breakpoint_list_t::size_type which) const {
        return breakpoint_list.at(which);
    }
    
    Breakpoint *get_breakpoint_by_id(std::size_t which) const;
    
    /** Returns a breakpoint by address.
        @param address The address to search for.
        @return A SmartPointer to the given breakpoint. Not valid if no breakpoint exists at @a address.
    */
    Breakpoint *get_breakpoint_by_address(Word address) const;
    
    void handle_breakpoint();
    
    void handle_signal();
    /** Continues execution, with a given signal sent to the child.
        @param signal The signal to send; none if zero.
    */
    void continue_execution(int signal = 0);
    /** Tells the child to execute a single instruction. */
    void single_step();
    
    void add_signal_observer(SignalObserver *new_observer) {
        signal_observer_list.push_back(new_observer);
    }
    
    MapParser *get_map_parser() const { return map_parser; }
    
    /** Reads the memory map in /proc for the child process to determine the address a library is lodaded into.
        @note This function is expensive, do not overuse it.
        @param unique_identifer A string that uniquely identifies the library, such as its path.
        @return The address of the library, or 0 if no such library was found.
    */
    Word get_lib_offset(std::string unique_identifer);
    
#ifdef USE_OVERLOAD
    int get_pipe_fd() const { return pipe_fd; }
#endif
};

} // namespace PTrace

#endif
