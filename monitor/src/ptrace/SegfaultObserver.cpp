#include <signal.h>
#include "SegfaultObserver.h"
#include "Initializer.h"
#include "misc/Message.h"

namespace PTrace {

bool SegfaultObserver::handle_signal(int signal, int status) {
    if(signal == SIGSEGV) {
        Initializer::get_instance()->get_program_manager()->set_running(false);
        Misc::Message::Message(Misc::Message::WARNING_MESSAGE, "Program received SIGSEGV, aborting");
        return true;
    }
    
    return false;
}

} // namespace PTrace
