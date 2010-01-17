#include "MallocObserver.h"
#include "Initializer.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

bool MallocObserver::handle_breakpoint(Misc::SmartPointer<Breakpoint> breakpoint) {
    /*Misc::SmartPointer<ELF::Symbol> malloc_symbol = Initializer::get_instance()->get_program_manager()->get_elf_parser()->get_symbol("malloc");
    
    if(malloc_symbol.is_valid() && breakpoint->get_address() != malloc_symbol->get_address()) {
        return false;
    }
    std::cout << "Malloc breakpoint found!" << std::endl;
    return true;*/
    return false;
}

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon
