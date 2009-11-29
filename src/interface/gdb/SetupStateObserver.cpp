#include "SetupStateObserver.h"
#include "interface/Initializer.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

bool SetupStateObserver::notify(State from, State to) {
    if(!(from == State::START && to == State::SETUP)) return false;
    
    Misc::SmartPointer<Controller> controller = Interface::Initializer::get_instance()->get_controller();
    
    controller->send_command("-break-insert _start");
    controller->send_command("-exec-run");
    
    /* Parse symbols, set breakpoints. */
    
    controller->send_command("-exec-continue");
    
    controller->set_state(State::RUNNING);
    
    return true;
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
