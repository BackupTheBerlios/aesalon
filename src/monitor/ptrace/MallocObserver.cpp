#include "MallocObserver.h"
#include "Initializer.h"
/*#include "platform/MemoryEvent.h"*/
#include "BreakpointReference.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

bool MallocObserver::handle_breakpoint(const BreakpointReference &breakpoint) {
    std::cout << "MallocObserver::handle_breakpoint(): asked to handle breakpoint ID #" << breakpoint->get_id() << std::endl;
    Misc::SmartPointer<ELF::Symbol> malloc_symbol = Initializer::get_instance()->get_program_manager()->get_libc_parser()->get_symbol("malloc");
    Misc::SmartPointer<Portal> portal = Initializer::get_instance()->get_program_manager()->get_ptrace_portal();
    
    static Word last_size = 0;
    
    if(breakpoint->get_id() != Initializer::get_instance()->get_program_manager()->get_malloc_breakpoint_id()) {
    /*if(malloc_symbol.is_valid() && breakpoint->get_address() != (malloc_symbol->get_address()
        + Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->get_libc_offset())) {*/
        
        std::cout << "* MallocObserver::handle_breakpoint(): return value from malloc() is 0x"
            << std::hex << portal->get_register(ASM::Register::RAX) << std::endl;
        breakpoint->set_valid(false);
/*        Initializer::get_instance()->get_event_queue()->push_event(new Platform::MemoryBlockAllocEvent(portal->get_register(ASM::Register::RAX), last_size));*/
        
        return true;
    }
    static int called_times = 0;
    /* NOTE: malloc() calls malloc() for some reason, so skip it. */
    /* TODO: figure out why this happens, and find a workaround. */
    if(called_times++ % 2) return true;
    std::cout << "MallocObserver::handle_breakpoint(): malloc breakpoint found . . ." << std::endl;
    Word rsp = portal->get_register(ASM::Register::RSP);
    std::cout << "\tRBP is: " << std::hex << rsp << std::endl;
    Word return_address = portal->read_memory(rsp);
    /* NOTE: rbp-40 is *AN EXTEREMELY BAD IDEA*! *don't* trust it . . . */
    /* TODO: find where the return address is really being stored . . . */
    return_address = portal->read_memory(rsp);
    std::cout << "\tReturn address: " << return_address << std::endl;
    breakpoints.insert(portal->place_breakpoint(return_address, this));
    std::cout << "\tMemory block size will be " << portal->get_register(ASM::Register::RDI) << std::endl;
    last_size = portal->get_register(ASM::Register::RDI);
    
    return true;
}

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon
