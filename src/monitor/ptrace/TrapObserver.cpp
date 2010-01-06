#include <signal.h>
#include <iostream>

#include "TrapObserver.h"
#include "Initializer.h"
#include "PTraceException.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

bool TrapObserver::handle_signal(int signal, int status) {
    if(signal != SIGTRAP) return false;
    
    Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->handle_breakpoint();
    Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->continue_execution();
    return true;
}

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon
