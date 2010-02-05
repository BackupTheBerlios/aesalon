#include <iostream>

#include "MallocObserver.h"
#include "Initializer.h"
#include "event/BlockEvent.h"

namespace PTrace {

bool MallocObserver::handle_breakpoint(Breakpoint *breakpoint) {
    Portal *portal = Initializer::get_instance()->get_program_manager()->get_ptrace_portal();
    
    Word size, address, ret_address;
    Word rbp = portal->get_register(ASM::Register::RBP);
    
    address = portal->read_memory(rbp-0x08);
    size = portal->read_memory(rbp-0x18);
    ret_address = portal->read_memory(rbp-0x20);
    
    Initializer::get_instance()->get_event_queue()->push_event(new Event::BlockEvent(Event::BlockEvent::ALLOC_EVENT, address, size));
    
    return true;
}

} // namespace PTrace
