#include <iostream>
#include <signal.h>
#include "ProgramManager.h"
#include "Initializer.h"

ProgramManager::ProgramManager(Misc::ArgumentList *argument_list)
    : argument_list(argument_list), running(false), ptrace_portal(NULL) {
    
    elf_parser = new ELF::Parser(argument_list->get_argument(0));
    libc_parser = new ELF::Parser(Initializer::get_instance()->get_argument_parser()->get_argument("libc-path")->get_data());
    disassembler = new ASM::Disassembler(elf_parser);
    
    /*disassembler->get_symbol_il("main");*/
}

ProgramManager::~ProgramManager() {
    if(ptrace_portal) delete ptrace_portal;
    delete elf_parser;
    delete libc_parser;
    delete disassembler;
    delete argument_list;
}

void ProgramManager::execute() {
    running = true;
    ptrace_portal = new PTrace::Portal(get_argument_list());
    /*std::cout << "\tlibc offset is: " << get_ptrace_portal()->get_libc_offset() << std::endl;
    std::cout << "\ttherefore, address of malloc is: " << libc_parser->get_symbol("malloc")->get_address() + get_ptrace_portal()->get_libc_offset() << std::endl;*/
}

void ProgramManager::wait() {
    ptrace_portal->handle_signal();
}

void ProgramManager::place_initial_breakpoints() {
    std::cout << std::hex;
    Word libc_offset = get_ptrace_portal()->get_libc_offset();
    Word malloc_address = libc_offset + get_libc_parser()->get_symbol("malloc")->get_address();
    malloc_breakpoint_id = get_ptrace_portal()->place_breakpoint(malloc_address, get_ptrace_portal()->get_malloc_observer());
    
    Word free_address = libc_offset + get_libc_parser()->get_symbol("free")->get_address();
    free_breakpoint_id = get_ptrace_portal()->place_breakpoint(free_address, get_ptrace_portal()->get_free_observer());
    
    Word realloc_address = libc_offset + get_libc_parser()->get_symbol("realloc")->get_address();
    realloc_breakpoint_id = get_ptrace_portal()->place_breakpoint(realloc_address, get_ptrace_portal()->get_realloc_observer());
    
    /* Remove the breakpoint on main(), it's not required any more. */
    /*get_ptrace_portal()->get_breakpoint_by_address(get_elf_parser()->get_symbol("main")->get_address())->set_valid(false);*/
}
