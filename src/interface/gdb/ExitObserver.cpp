#include <sstream>
#include "ExitObserver.h"
#include "StringFollower.h"
#include "interface/Initializer.h"
#include "misc/String.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

bool ExitObserver::notify(Misc::SmartPointer<AsyncOutput> async) {
    if(async->get_data()->get_first() == "stopped") {
        std::string reason = StringFollower(async).follow("'reason' rhs");
        if(reason == "exited-normally" || reason == "exited") {
            set_alive(false);
            Initializer::get_instance()->get_controller()->set_running(false);
            if(reason == "exited") {
                int return_value;
                Misc::String::to<int>(StringFollower(async).follow("'exit-code' rhs"), return_value);
                Initializer::get_instance()->set_return_value(return_value);
            }
            else Initializer::get_instance()->set_return_value(0);
            return true;
        }
    }
    return false;
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
