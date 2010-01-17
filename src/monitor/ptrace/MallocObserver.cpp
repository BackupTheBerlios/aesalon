#include "MallocObserver.h"
#include "Initializer.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

bool MallocObserver::handle_breakpoint(Misc::SmartPointer<Breakpoint> breakpoint) {
    Misc::SmartPointer<ELF::Symbol> malloc_symbol = Initializer::get_instance()->get_program_manager()->get_libc_parser()->get_symbol("malloc");
    Misc::SmartPointer<Portal> portal = Initializer::get_instance()->get_program_manager()->get_ptrace_portal();
    
    if(malloc_symbol.is_valid() && breakpoint->get_address() != (malloc_symbol->get_address()
        + Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->get_libc_offset())) {
        
        breakpoint_set_t::iterator i = breakpoints.find(breakpoint->get_id());
        if(i != breakpoints.end()) {
            std::cout << "Return value from malloc() is:" << std::hex << portal->get_register(ASM::Register::RAX) << std::endl;
        }
        
        return false;
    }
    std::cout << "MallocObserver::handle_breakpoint(): malloc breakpoint found . . ." << std::endl;
    Word rbp = portal->get_register(ASM::Register::RBP);
    std::cout << "\tRBP is: " << std::hex << rbp << std::endl;
    Word return_address = portal->read_memory(rbp);
    /* NOTE: rbp-40 is *AN EXTEREMELY BAD IDEA*! *don't* trust it . . . */
    /* TODO: find where the return address is really being stored . . . */
    return_address = portal->read_memory(rbp-40);
    std::cout << "\tReturn address: " << return_address << std::endl;
    breakpoints.insert(portal->place_breakpoint(return_address));
    
    return true;
}

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon
