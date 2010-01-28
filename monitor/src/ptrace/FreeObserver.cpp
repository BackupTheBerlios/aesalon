#include <iostream>
#include "FreeObserver.h"
#include "Initializer.h"
#include "BreakpointReference.h"
#include "Portal.h"
#include "misc/BlockEvent.h"





void FreeObserver::handle_breakpoint(const BreakpointReference &breakpoint) {
    Initializer::get_instance()->get_event_queue()->push_event(
            new Misc::BlockEvent(Misc::BlockEvent::FREE_EVENT,
            Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->get_register(ASM::Register::RAX)));
}




