#include <signal.h>

#include "ExitObserver.h"
#include "Initializer.h"

namespace PTrace {

bool ExitObserver::handle_signal(int signal, int status) {
    if(signal == SIGKILL || signal == -1) {
        Initializer::get_instance()->get_program_manager()->set_running(false);
        return true;
    }
    return false;
}

} // namespace PTrace
