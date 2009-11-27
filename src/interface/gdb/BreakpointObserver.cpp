#include "BreakpointObserver.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

bool BreakpointObserver::notify(Misc::SmartPointer<String> string, Misc::SmartPointer<StateManager> state_manager) {
    if(state_manager->get_state() != StateManager::RUNNING) return false;
    Misc::SmartPointer<AsyncOutput> async = string.to<AsyncOutput>();
    
    if(async->get_data()->get_first() != "stopped") return false;
    try {
        if(async->get_data()->get_element("reason").to<ParseResult>()->get_value().to<ParseString>()->get_data() == "") {}
    }
    catch(Misc::InvalidCastException ice) { return false; }
    catch(Misc::NullPointerException npe) { return false; }
    
    return false;
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
