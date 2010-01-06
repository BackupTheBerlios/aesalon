#include "MallocObserver.h"
#include "Initializer.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

bool MallocObserver::handle_breakpoint(Misc::SmartPointer<Breakpoint> breakpoint) {
    if(breakpoint->get_address() != Initializer::get_instance()->get_program_manager()->get_elf_parser()->get_symbol("malloc")->get_address()) {
        
        return false;
    }
    std::cout << "Malloc breakpoint found!" << std::endl;
    return true;
}

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon
