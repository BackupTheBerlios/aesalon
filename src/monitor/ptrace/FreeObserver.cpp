#include <iostream>
#include "FreeObserver.h"
#include "Initializer.h"
#include "BreakpointReference.h"
#include "Portal.h"
#include "platform/BlockEvent.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

void FreeObserver::handle_breakpoint(const BreakpointReference &breakpoint) {
    Initializer::get_instance()->get_event_queue()->push_event(
            new Platform::BlockEvent(Platform::BlockEvent::FREE_EVENT,
            Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->get_register(ASM::Register::RAX)));
}

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon
