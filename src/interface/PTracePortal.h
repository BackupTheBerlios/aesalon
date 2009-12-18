#ifndef AESALON_INTERFACE_PTRACE_PORTAL_H
#define AESALON_INTERFACE_PTRACE_PORTAL_H

#include <sys/types.h>
#include <sys/user.h>

#include <vector>

#include "Breakpoint.h"
#include "Types.h"
#include "platform/MemoryAddress.h"
#include "misc/SmartPointer.h"

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
private:
    pid_t pid;
    
    breakpoint_list_t breakpoint_list;
    
    void add_breakpoint(Misc::SmartPointer<Breakpoint> breakpoint) {
        breakpoint_list.push_back(breakpoint);
    }
public:
    PTracePortal(pid_t pid) : pid(pid) {}
    virtual ~PTracePortal() {}
    
    /*void temp() {
        struct user_regs_struct regs;
        regs.
    }*/
    
    Platform::MemoryAddress get_register(register_e which) const;
    
    Word read_memory(Platform::MemoryAddress address) const;
    void write_memory(Platform::MemoryAddress address, Word value);
    void write_memory(Platform::MemoryAddress address, Byte value);
    
    void attach();
    
    void place_breakpoint(Platform::MemoryAddress address);
    
    Misc::SmartPointer<Breakpoint> get_breakpoint(breakpoint_list_t::size_type which) const {
        return breakpoint_list.at(which);
    }
    Misc::SmartPointer<Breakpoint> get_breakpoint_by_address(Platform::MemoryAddress address) const;
};

} // namespace Interface
} // namespace Aesalon

#endif
