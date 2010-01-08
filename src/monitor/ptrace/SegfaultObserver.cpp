#include <signal.h>
#include "SegfaultObserver.h"
#include "Initializer.h"
#include "Message.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

bool SegfaultObserver::handle_signal(int signal, int status) {
    if(signal == SIGSEGV) {
        Initializer::get_instance()->get_program_manager()->set_running(false);
        Message::Message(Message::WARNING_MESSAGE, "Program segfaulted");
        return true;
    }
    return false;
}

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon
