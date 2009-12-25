#include <signal.h>
#include "ProgramManager.h"

namespace Aesalon {
namespace Interface {

ProgramManager::ProgramManager(Misc::SmartPointer<Platform::ArgumentList> argument_list)
    : argument_list(argument_list), running(false) {
    
    elf_parser = new ELF::Parser(argument_list->get_argument(0));
    dwarf_parser = new DWARF::Parser(elf_parser);
}

void ProgramManager::execute() {
    running = true;
    ptrace_portal = new PTrace::Portal(get_argument_list());
}

void ProgramManager::wait() {
    ptrace_portal->handle_signal();
}

} // namespace Interface
} // namespace Aesalon
