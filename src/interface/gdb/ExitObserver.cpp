#include "ExitObserver.h"
#include "StringFollower.h"
#include "interface/Initializer.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

bool ExitObserver::notify(Misc::SmartPointer<AsyncOutput> async) {
    if(async->get_data()->get_first() == "stopped") {
        std::string reason = StringFollower(async).follow("'reason' rhs");
        if(reason == "exited-normally" || reason == "exited") {
            set_alive(false);
            Initializer::get_instance()->get_controller()->set_running(false);
            return true;
        }
    }
    return false;
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
