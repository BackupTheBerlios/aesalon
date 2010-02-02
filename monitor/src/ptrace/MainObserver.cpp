#include "MainObserver.h"
#include "Initializer.h"

namespace PTrace {

bool MainObserver::handle_breakpoint(Breakpoint *breakpoint) {
    Initializer::get_instance()->get_program_manager()->place_initial_breakpoints();
    breakpoint->remove_observer(this);
    return false;
}

} // namespace PTrace
