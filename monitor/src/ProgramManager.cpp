#include <iostream>
#include <signal.h>
#include "ProgramManager.h"

ProgramManager::ProgramManager(Misc::ArgumentList *argument_list)
    : argument_list(argument_list), running(false) {
    
    elf_parser = new ELF::Parser(argument_list->get_argument(0));
    /*std::string libc_path = Misc::ArgumentParser::get_instance()->get_argument("libc path").to<Misc::StringArgument>()->get_value();*/
    std::string libc_path = LIBC_PATH;
    libc_parser = new ELF::Parser(libc_path);
    disassembler = new ASM::Disassembler(elf_parser);
    
    disassembler->get_symbol_il("main");
}

void ProgramManager::execute() {
    std::cout << "ProgramManager::execute() called . . ." << std::endl;
    running = true;
    ptrace_portal = new PTrace::Portal(get_argument_list());
    /*std::cout << "\tlibc offset is: " << get_ptrace_portal()->get_libc_offset() << std::endl;
    std::cout << "\ttherefore, address of malloc is: " << libc_parser->get_symbol("malloc")->get_address() + get_ptrace_portal()->get_libc_offset() << std::endl;*/
}

void ProgramManager::wait() {
    ptrace_portal->handle_signal();
}

void ProgramManager::place_initial_breakpoints() {
    std::cout << "ProgramManager::place_initial_breakpoints() called . . ." << std::endl;
    std::cout << std::hex;
    Word libc_offset = get_ptrace_portal()->get_libc_offset();
    std::cout << "ProgramManager::place_initial_breakpoints(): placing malloc breakpoint . . ." << std::endl;
    std::cout << "\tlibc offset is " << libc_offset << std::endl;
    std::cout << "\tmalloc offset is " << get_libc_parser()->get_symbol("malloc")->get_address() << std::endl;
    Word malloc_address = libc_offset + get_libc_parser()->get_symbol("malloc")->get_address();
    std::cout << "\tmalloc address is " << malloc_address << std::endl;
    malloc_breakpoint_id = get_ptrace_portal()->place_breakpoint(malloc_address, get_ptrace_portal()->get_malloc_observer());
    
    std::cout << "ProgramManager::place_initial_breakpoints(): placing free breakpoint . . ." << std::endl;
    Word free_address = libc_offset + get_libc_parser()->get_symbol("free")->get_address();
    std::cout << "\tfree address is " << free_address << std::endl;
    free_breakpoint_id = get_ptrace_portal()->place_breakpoint(free_address, get_ptrace_portal()->get_free_observer());
    
    std::cout << "ProgramManager::place_initial_breakpoints(): placing realloc breakpoing . . ." << std::endl;
    Word realloc_address = libc_offset + get_libc_parser()->get_symbol("realloc")->get_address();
    std::cout << "\trealloc address is " << realloc_address << std::endl;
    realloc_breakpoint_id = get_ptrace_portal()->place_breakpoint(realloc_address, get_ptrace_portal()->get_realloc_observer());
    
    /* Remove the breakpoint on main(), it's not required any more. */
    /*get_ptrace_portal()->get_breakpoint_by_address(get_elf_parser()->get_symbol("main")->get_address())->set_valid(false);*/
}
