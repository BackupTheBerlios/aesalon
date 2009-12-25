#include <signal.h>

#include "TrapObserver.h"
#include "Initializer.h"

namespace Aesalon {
namespace Interface {
namespace PTrace {

bool TrapObserver::handle_signal(int signal, int status) {
    if(signal != SIGTRAP) return false;
    std::cout << "TrapObserver::handle_signal(): signal is SIGTRAP, continuing execution . . ." << std::endl;
    Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->continue_execution();
    return true;
}

} // namespace PTrace
} // namespace Interface
} // namespace Aesalon
