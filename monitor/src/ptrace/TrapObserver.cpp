#include <signal.h>
#include <iostream>

#include "TrapObserver.h"
#include "Initializer.h"
#include "PTraceException.h"





bool TrapObserver::handle_signal(int signal, int status) {
    if(signal != SIGTRAP) return false;
    ProgramManager *program_manager = Initializer::get_instance()->get_program_manager();
    
    std::cout << "TrapObserver::handle_signal(): calling handle_breakpoint() . . ." << std::endl;
    program_manager->get_ptrace_portal()->handle_breakpoint();
    std::cout << "TrapObserver::handle_signal(): calling continue_execution() . . ." << std::endl;
    program_manager->get_ptrace_portal()->continue_execution();
    return true;
}




