#include "MallocObserver.h"
#include "Initializer.h"
#include "event/BlockEvent.h"

namespace PTrace {

void MallocObserver::handle_breakpoint(Breakpoint *breakpoint) {
    Portal *portal = Initializer::get_instance()->get_program_manager()->get_ptrace_portal();
    
    static Word last_size = 0;
    
    if(breakpoint->get_id() != Initializer::get_instance()->get_program_manager()->get_malloc_breakpoint_id()) {
        breakpoint->remove_observer(this);
        Initializer::get_instance()->get_event_queue()->push_event(
            new Event::BlockEvent(Event::BlockEvent::ALLOC_EVENT,
            portal->get_register(ASM::Register::RAX), last_size));
        return;
    }
    Word rsp = portal->get_register(ASM::Register::RSP);
    Word return_address = portal->read_memory(rsp);
    /* NOTE: qword [rsp] is where the return address is stored in libc 2.10.2-5, but don't rely on it! */
    return_address = portal->read_memory(rsp);
    portal->place_breakpoint(return_address, this);
    last_size = portal->get_register(ASM::Register::RDI);
    
    return;
}

} // namespace PTrace
