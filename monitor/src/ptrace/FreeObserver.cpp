#include <iostream>
#include "FreeObserver.h"
#include "Initializer.h"
#include "Portal.h"
#include "event/Block.h"

namespace PTrace {

void FreeObserver::handle_breakpoint(Breakpoint *breakpoint) {
    Initializer::get_instance()->get_event_queue()->push_event(
            new Event::Block(Event::Block::FREE_EVENT,
            Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->get_register(ASM::Register::RAX)));
}

} // namespace PTrace
