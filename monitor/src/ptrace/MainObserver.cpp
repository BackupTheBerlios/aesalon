#include "MainObserver.h"
#include "Initializer.h"

namespace PTrace {

void MainObserver::handle_breakpoint(Breakpoint *breakpoint) {
    Initializer::get_instance()->get_program_manager()->place_initial_breakpoints();
    breakpoint->remove_observer(this);
}

} // namespace PTrace
