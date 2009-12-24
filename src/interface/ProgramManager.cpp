#include <signal.h>
#include "ProgramManager.h"

namespace Aesalon {
namespace Interface {

ProgramManager::ProgramManager(Misc::SmartPointer<Platform::ArgumentList> argument_list)
    : bi_pipe(NULL), argument_list(argument_list), running(false) {
    
    elf_parser = new ELF::Parser(argument_list->get_argument(0));
    dwarf_parser = new DWARF::Parser(elf_parser);
}

void ProgramManager::execute() {
    bi_pipe = new Platform::BidirectionalPipe(get_argument_list()->get_argument(0), get_argument_list(), false, true);
    running = true;
    ptrace_portal = new PTrace::Portal(bi_pipe->get_child_pid());
    /*ptrace_portal->attach();*/
}

void ProgramManager::wait() {
    ptrace_portal->wait_for_signal();
}

void ProgramManager::handle() {
}

} // namespace Interface
} // namespace Aesalon
