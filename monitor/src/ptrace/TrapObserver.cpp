#include <signal.h>
#include <iostream>

#include "TrapObserver.h"
#include "Initializer.h"

namespace PTrace {

bool TrapObserver::handle_signal(int signal, int status) {
    if(signal != SIGTRAP) return false;
    ProgramManager *program_manager = Initializer::get_instance()->get_program_manager();
    
    program_manager->get_ptrace_portal()->handle_breakpoint();
    program_manager->get_ptrace_portal()->continue_execution();
    return true;
}

} // namespace PTrace
