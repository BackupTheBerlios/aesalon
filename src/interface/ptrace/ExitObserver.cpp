#include <signal.h>

#include "ExitObserver.h"
#include "Initializer.h"

namespace Aesalon {
namespace Interface {
namespace PTrace {

bool ExitObserver::handle_signal(int signal, int status) {
    if(signal == SIGKILL) {
        Initializer::get_instance()->get_program_manager()->set_running(false);
        return true;
    }
    return false;
}

} // namespace PTrace
} // namespace Interface
} // namespace Aesalon
