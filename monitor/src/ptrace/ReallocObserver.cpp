#include <iostream>
#include "ReallocObserver.h"
#include "event/BlockEvent.h"
#include "Initializer.h"

namespace PTrace {

bool ReallocObserver::handle_breakpoint(Breakpoint *breakpoint) {
    Portal *portal = Initializer::get_instance()->get_program_manager()->get_ptrace_portal();
    Word rbp = portal->get_register(ASM::Register::RBP);
    Word size, address, new_address, ret_address;
    
    new_address = portal->read_memory(rbp-0x08);
    address = portal->read_memory(rbp-0x18);
    size = portal->read_memory(rbp-0x20);
    ret_address = portal->read_memory(rbp-0x28);
    
    Initializer::get_instance()->get_event_queue()->push_event(new Event::BlockEvent(Event::BlockEvent::REALLOC_EVENT, address, size, new_address));
    
    return true;
}

} // namesapce PTrace
