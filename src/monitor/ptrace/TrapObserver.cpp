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
    static bool first_run = true;
    Misc::SmartPointer<ProgramManager> program_manager = Initializer::get_instance()->get_program_manager();
    
    if(first_run) {
        std::cout << "\tTrapObserver::handle_signal(): libc offset is " << program_manager->get_ptrace_portal()->get_libc_offset() << std::endl;
        /*program_manager->get_ptrace_portal()->place_breakpoint(program_manager->get_elf_parser()->get_symbol("malloc")->get_address());*/
        first_run = false;
        program_manager->get_ptrace_portal()->continue_execution();
        return true;
    }
    std::cout << "TrapObserver::handle_signal(): calling handle_breakpoint() . . ." << std::endl;
    program_manager->get_ptrace_portal()->handle_breakpoint();
    std::cout << "TrapObserver::handle_signal(): calling continue_execution() . . ." << std::endl;
    program_manager->get_ptrace_portal()->continue_execution();
    return true;
}

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon
