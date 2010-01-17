#include "MallocObserver.h"
#include "Initializer.h"
#include "platform/MemoryEvent.h"
#include "BreakpointReference.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

bool MallocObserver::handle_breakpoint(const BreakpointReference &breakpoint) {
    Misc::SmartPointer<ELF::Symbol> malloc_symbol = Initializer::get_instance()->get_program_manager()->get_libc_parser()->get_symbol("malloc");
    Misc::SmartPointer<Portal> portal = Initializer::get_instance()->get_program_manager()->get_ptrace_portal();
    
    static Word last_size = 0;
    
    if(malloc_symbol.is_valid() && breakpoint->get_address() != (malloc_symbol->get_address()
        + Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->get_libc_offset())) {
        
        breakpoint_set_t::iterator i = breakpoints.find(breakpoint->get_id());
        if(i == breakpoints.end()) return false;
        
        std::cout << "Return value from malloc() is:" << std::hex << portal->get_register(ASM::Register::RAX) << std::endl;
        breakpoint->set_valid(false);
        Initializer::get_instance()->get_event_queue()->push_event(new Platform::MemoryBlockAllocEvent(portal->get_register(ASM::Register::RAX), last_size));
        
        return true;
    }
    static int called_times = 0;
    /* NOTE: malloc() calls malloc() for some reason, so skip it. */
    /* TODO: figure out why this happens, and find a workaround. */
    if(called_times++ % 2) return true;
    std::cout << "MallocObserver::handle_breakpoint(): malloc breakpoint found . . ." << std::endl;
    Word rbp = portal->get_register(ASM::Register::RBP);
    std::cout << "\tRBP is: " << std::hex << rbp << std::endl;
    Word return_address = portal->read_memory(rbp);
    /* NOTE: rbp-40 is *AN EXTEREMELY BAD IDEA*! *don't* trust it . . . */
    /* TODO: find where the return address is really being stored . . . */
    return_address = portal->read_memory(rbp-40);
    std::cout << "\tReturn address: " << return_address << std::endl;
    breakpoints.insert(portal->place_breakpoint(return_address, this));
    std::cout << "\tMemory block size will be " << portal->get_register(ASM::Register::RDI) << std::endl;
    last_size = portal->get_register(ASM::Register::RDI);
    
    return true;
}

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon
