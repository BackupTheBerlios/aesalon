#include "ExitObserver.h"
#include "interface/Initializer.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

bool ExitObserver::notify(Misc::SmartPointer<String> string, Misc::SmartPointer<StateManager> state_manager) {
    
    return false;
}


} // namespace GDB
} // namespace Interface
} // namespace Aesalon
