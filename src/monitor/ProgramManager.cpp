#include <iostream>
#include <signal.h>
#include "ProgramManager.h"

namespace Aesalon {
namespace Monitor {

ProgramManager::ProgramManager(Misc::SmartPointer<Platform::ArgumentList> argument_list)
    : argument_list(argument_list), running(false) {
    
    elf_parser = new ELF::Parser(argument_list->get_argument(0));
    dwarf_parser = new DWARF::Parser(elf_parser);
    disassembler = new ASM::Disassembler(elf_parser);
    
    disassembler->get_symbol_il("main");
}

void ProgramManager::execute() {
    running = true;
    ptrace_portal = new PTrace::Portal(get_argument_list());
    /*elf_parser->dump_symbols();*/
    std::cout << "ProgramManager: address of malloc is: " << elf_parser->get_symbol("malloc")->get_address() << std::endl;
}

void ProgramManager::wait() {
    ptrace_portal->handle_signal();
}

} // namespace Monitor
} // namespace Aesalon
