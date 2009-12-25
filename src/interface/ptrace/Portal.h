#ifndef AESALON_INTERFACE_PTRACE_PORTAL_H
#define AESALON_INTERFACE_PTRACE_PORTAL_H

#include <sys/types.h>
#include <sys/user.h>

#include <vector>

#include "Breakpoint.h"
#include "Types.h"
#include "SignalObserver.h"
#include "platform/MemoryAddress.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
namespace PTrace {

class PTraceException : public Misc::Exception {
public:
    PTraceException(std::string message) : Misc::Exception(message) {}
    virtual ~PTraceException() {}
};

class Portal {
public:
    /** An enum representing the different registers available. */
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
        RIP,
        RDI,
        RSI,
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
        EAX,
        EBX,
        ECX,
        EDX,
        EBP,
        ESP,
        EIP,
        EDI,
        ESI,
#endif
        CS,
        SS
    };
    
    typedef std::vector<Misc::SmartPointer<Breakpoint> > breakpoint_list_t;
    
    typedef std::vector<Misc::SmartPointer<SignalObserver> > signal_observer_list_t;
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
    
    int wait_for_signal();
public:
    /** Generic constructor for PTracePortal.
        @param pid The PID of the child process.
    */
    Portal(pid_t pid);
    /** Virtual destructor, does nothing. */
    virtual ~Portal() {}
    
    /** Retrieve the value of a specific register.
        @param which The register to get the value of.
        @return The value of the specified register.
    */
    Platform::MemoryAddress get_register(register_e which) const;
    
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
    
    /** Places a breakpoint at a specified address.
        @param address The address to place the breakpoint at.
    */
    void place_breakpoint(Platform::MemoryAddress address);
    
    /** Returns a breakpoint by iterator.
        @param which The breakpoint to find.
        @return A SmartPointer to the given breakpoint. Not valid if the breakpoint iterator does not exist.
    */
    Misc::SmartPointer<Breakpoint> get_breakpoint(breakpoint_list_t::size_type which) const {
        return breakpoint_list.at(which);
    }
    /** Returns a breakpoint by address.
        @param address The address to search for.
        @return A SmartPointer to the given breakpoint. Not valid if no breakpoint exists at @a address.
    */
    /*Misc::SmartPointer<Breakpoint> get_breakpoint_by_address(Platform::MemoryAddress address) const;*/
    
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
};

} // namespace PTrace
} // namespace Interface
} // namespace Aesalon

#endif
