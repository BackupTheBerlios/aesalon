#include "MainObserver.h"
#include "Initializer.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

bool MainObserver::handle_breakpoint(Misc::SmartPointer<Breakpoint> breakpoint) {
    static bool first_run = true;
    if(first_run) {
        first_run = false;
        Initializer::get_instance()->get_program_manager()->place_initial_breakpoints();
        return true;
    }
    return false;
}

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon
