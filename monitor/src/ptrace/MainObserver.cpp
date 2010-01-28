#include "MainObserver.h"
#include "Initializer.h"
#include "BreakpointReference.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

void MainObserver::handle_breakpoint(const BreakpointReference &breakpoint) {
    Initializer::get_instance()->get_program_manager()->place_initial_breakpoints();
    breakpoint->remove_observer(this);
}

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon
