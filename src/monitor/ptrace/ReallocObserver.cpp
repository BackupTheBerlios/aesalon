#include <iostream>
#include "ReallocObserver.h"
#include "BreakpointReference.h"
#include "platform/BlockEvent.h"
#include "Initializer.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

void ReallocObserver::handle_breakpoint(const BreakpointReference &breakpoint) {
    Misc::SmartPointer<Portal> portal = Initializer::get_instance()->get_program_manager()->get_ptrace_portal();
    
    static Word last_size = 0;
    static Word last_address = 0;
    
    if(breakpoint->get_id() != Initializer::get_instance()->get_program_manager()->get_realloc_breakpoint_id()) {
        breakpoint->remove_observer(this);
        Initializer::get_instance()->get_event_queue()->push_event(
            new Platform::BlockEvent(Platform::BlockEvent::REALLOC_EVENT,
            last_address, last_size, portal->get_register(ASM::Register::RAX)));
        return;
    }
    Word rsp = portal->get_register(ASM::Register::RSP);
    Word return_address = portal->read_memory(rsp);
    /* NOTE: qword [rsp] is where the return address is stored in libc 2.10.2-5, but don't rely on it! */
    return_address = portal->read_memory(rsp);
    portal->place_breakpoint(return_address, this);
    last_size = portal->get_register(ASM::Register::RSI);
    last_address = portal->get_register(ASM::Register::RDI);
    return;
}

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon
