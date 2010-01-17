#ifndef AESALON_MONITOR_PTRACE_PORTAL_H
#define AESALON_MONITOR_PTRACE_PORTAL_H

#include <sys/types.h>
#include <sys/user.h>

#include <vector>

#include "Breakpoint.h"
#include "Types.h"
#include "SignalObserver.h"
#include "BreakpointObserver.h"
#include "platform/MemoryAddress.h"
#include "platform/ArgumentList.h"
#include "misc/SmartPointer.h"
#include "PTraceException.h"
#include "asm/Register.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

class Portal {
protected:
    typedef std::vector<Misc::SmartPointer<Breakpoint> > breakpoint_list_t;
    
    typedef std::vector<Misc::SmartPointer<SignalObserver> > signal_observer_list_t;
    typedef std::vector<Misc::SmartPointer<BreakpointObserver> > breakpoint_observer_list_t;
private:
    /** The PID of the attached process. */
    pid_t pid;
    
    /** The vector of breakpoints. */
    breakpoint_list_t breakpoint_list;
    
    /** Adds a breakpoint onto the list.
        @param breakpoint The breakpoint to add.
    */
    void add_breakpoint(Misc::SmartPointer<Breakpoint> breakpoint) {
        breakpoint_list.push_back(breakpoint);
    }
    
    signal_observer_list_t signal_observer_list;
    
    breakpoint_observer_list_t breakpoint_observer_list;
    
    Word libc_offset;
    
    int wait_for_signal();
public:
    /** Generic constructor for PTracePortal.
        @param argument_list The arguments to spawn the child with.
    */
    Portal(Misc::SmartPointer<Platform::ArgumentList> argument_list);
    /** Virtual destructor, does nothing. */
    virtual ~Portal() {}
    
    /** Retrieve the value of a specific register.
        @param which The register to get the value of.
        @return The value of the specified register.
    */
    Platform::MemoryAddress get_register(ASM::Register which) const;
    
    void set_register(ASM::Register which, Word new_value);
    
    /** Read a Word of memory.
        @param address The address to read.
        @return The value at @a address.
    */
    Word read_memory(Platform::MemoryAddress address) const;
    /** Write a Word of memory.
        @param address The address to write to.
        @param value The value to write.
    */
    void write_memory(Platform::MemoryAddress address, Word value);
    /** Write a Byte of memory.
        @param address The address to write to.
        @param value The value to write.
    */
    void write_memory(Platform::MemoryAddress address, Byte value);
    
    /** Attaches onto the PID @a pid. */
    void attach();
    /** Detaches from running program. */
    void detach();
    
    /** Places a breakpoint at a specified address.
        @param address The address to place the breakpoint at.
        @return The ID of the newly-inserted breakpoint.
    */
    std::size_t place_breakpoint(Platform::MemoryAddress address);
    
    void remove_breakpoint(Platform::MemoryAddress address);
    
    /** Returns a breakpoint by iterator.
        @param which The breakpoint to find.
        @return A SmartPointer to the given breakpoint. Not valid if the breakpoint iterator does not exist.
    */
    Misc::SmartPointer<Breakpoint> get_breakpoint(breakpoint_list_t::size_type which) const {
        return breakpoint_list.at(which);
    }
    
    Misc::SmartPointer<Breakpoint> get_breakpoint_by_id(std::size_t which) const;
    
    /** Returns a breakpoint by address.
        @param address The address to search for.
        @return A SmartPointer to the given breakpoint. Not valid if no breakpoint exists at @a address.
    */
    Misc::SmartPointer<Breakpoint> get_breakpoint_by_address(Platform::MemoryAddress address) const;
    
    void handle_breakpoint();
    
    void handle_signal();
    /** Continues execution, with a given signal sent to the child.
        @param signal The signal to send; none if zero.
    */
    void continue_execution(int signal = 0);
    /** Tells the child to execute a single instruction. */
    void single_step();
    
    void add_signal_observer(Misc::SmartPointer<SignalObserver> new_observer) {
        signal_observer_list.push_back(new_observer);
    }
    void add_breakpoint_observer(Misc::SmartPointer<BreakpointObserver> new_observer) {
        breakpoint_observer_list.push_back(new_observer);
    }
    
    /** Reads the memory map in /proc for the child process to determine the address libc is lodaded into.
        @return The address of libc, or 0 if libc is not currently loaded.
    */
    Word get_libc_offset();
    
    void wait_for_breakpoint(Misc::SmartPointer<Breakpoint> breakpoint);
};

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon

#endif
