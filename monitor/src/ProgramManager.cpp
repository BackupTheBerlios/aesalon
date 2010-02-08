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
    /*Word libc_offset = get_ptrace_portal()->get_libc_offset();
    Word malloc_address = libc_offset + get_libc_parser()->get_symbol("malloc")->get_address();
    malloc_breakpoint_id = get_ptrace_portal()->place_breakpoint(malloc_address, get_ptrace_portal()->get_malloc_observer());
    
    Word free_address = libc_offset + get_libc_parser()->get_symbol("free")->get_address();
    free_breakpoint_id = get_ptrace_portal()->place_breakpoint(free_address, get_ptrace_portal()->get_free_observer());
    
    Word realloc_address = libc_offset + get_libc_parser()->get_symbol("realloc")->get_address();
    realloc_breakpoint_id = get_ptrace_portal()->place_breakpoint(realloc_address, get_ptrace_portal()->get_realloc_observer());*/
    
    std::string overload_path = Initializer::get_instance()->get_argument_parser()->get_argument("overload-path")->get_data();
    overload_parser = new ELF::Parser(overload_path);
    if(overload_parser == NULL) return;
    Word overload_offset = get_ptrace_portal()->get_lib_offset(overload_path.substr(overload_path.find_last_of('/')));
    
    /* TODO: find a better way than adding hard-coded values to get the offset of the int3 instruction . . . */
    std::cout << "overload_offset: " << overload_offset << std::endl;
    PTrace::Breakpoint *bp = new PTrace::Breakpoint(overload_offset + overload_parser->get_symbol("aesalon_malloc_hook")->get_address()+69, 0xcc);
    get_ptrace_portal()->add_breakpoint(bp);
    bp->add_observer(get_ptrace_portal()->get_malloc_observer());
    
    bp = new PTrace::Breakpoint(overload_offset + overload_parser->get_symbol("aesalon_realloc_hook")->get_address()+80, 0xcc);
    get_ptrace_portal()->add_breakpoint(bp);
    bp->add_observer(get_ptrace_portal()->get_realloc_observer());
    
    bp = new PTrace::Breakpoint(overload_offset + overload_parser->get_symbol("aesalon_free_hook")->get_address()+65, 0xcc);
    get_ptrace_portal()->add_breakpoint(bp);
    bp->add_observer(get_ptrace_portal()->get_free_observer());
}
