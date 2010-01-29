#include "MallocObserver.h"
#include "Initializer.h"
#include "event/BlockEvent.h"

namespace PTrace {

void MallocObserver::handle_breakpoint(Breakpoint *breakpoint) {
    std::cout << "MallocObserver::handle_breakpoint(): asked to handle breakpoint ID #" << breakpoint->get_id() << std::endl;
    Portal *portal = Initializer::get_instance()->get_program_manager()->get_ptrace_portal();
    
    static Word last_size = 0;
    
    if(breakpoint->get_id() != Initializer::get_instance()->get_program_manager()->get_malloc_breakpoint_id()) {
        std::cout << "* MallocObserver::handle_breakpoint(): return value from malloc() is 0x"
            << std::hex << portal->get_register(ASM::Register::RAX) << std::endl;
        breakpoint->remove_observer(this);
        Initializer::get_instance()->get_event_queue()->push_event(
            new Event::BlockEvent(Event::BlockEvent::ALLOC_EVENT,
            portal->get_register(ASM::Register::RAX), last_size));
        return;
    }
    std::cout << "MallocObserver::handle_breakpoint(): malloc breakpoint found . . ." << std::endl;
    Word rsp = portal->get_register(ASM::Register::RSP);
    std::cout << "\tRBP is: " << std::hex << rsp << std::endl;
    Word return_address = portal->read_memory(rsp);
    /* NOTE: qword [rsp] is where the return address is stored in libc 2.10.2-5, but don't rely on it! */
    return_address = portal->read_memory(rsp);
    std::cout << "\tReturn address: " << return_address << std::endl;
    portal->place_breakpoint(return_address, this);
    std::cout << "\tMemory block size will be " << portal->get_register(ASM::Register::RDI) << std::endl;
    last_size = portal->get_register(ASM::Register::RDI);
    
    return;
}

} // namespace PTrace
