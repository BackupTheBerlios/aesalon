#include <iostream>
#include "FreeObserver.h"
#include "Initializer.h"
#include "Portal.h"
#include "event/BlockEvent.h"

namespace PTrace {

bool FreeObserver::handle_breakpoint(Breakpoint *breakpoint) {
    Portal *portal = Initializer::get_instance()->get_program_manager()->get_ptrace_portal();
    Word address, ret_address;
    Word rbp = portal->get_register(ASM::Register::RBP);
    
    address = portal->read_memory(rbp-0x08);
    ret_address = portal->read_memory(rbp-0x10);
    
    Initializer::get_instance()->get_event_queue()->push_event(new Event::BlockEvent(Event::BlockEvent::FREE_EVENT, address));

    /*Initializer::get_instance()->get_event_queue()->push_event(
        new Event::BlockEvent(Event::BlockEvent::FREE_EVENT,
        Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->get_register(ASM::Register::RAX)));*/
    return true;
}

} // namespace PTrace
