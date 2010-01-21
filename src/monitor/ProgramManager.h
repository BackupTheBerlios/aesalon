#ifndef AESALON_MONITOR_PROGRAM_MANAGER_H
#define AESALON_MONITOR_PROGRAM_MANAGER_H

#include "misc/SmartPointer.h"
#include "platform/BidirectionalPipe.h"
#include "platform/ArgumentList.h"
#include "ptrace/Portal.h"
#include "elf/Parser.h"
#include "dwarf/Parser.h"
#include "asm/Disassembler.h"

namespace Aesalon {
namespace Monitor {

class ProgramManager {
private:
    Misc::SmartPointer<Platform::ArgumentList> argument_list;
    
    bool running;
    
    Misc::SmartPointer<PTrace::Portal> ptrace_portal;
    
    Misc::SmartPointer<ELF::Parser> elf_parser, libc_parser;
    Misc::SmartPointer<DWARF::Parser> dwarf_parser;
    Misc::SmartPointer<ASM::Disassembler> disassembler;
    
    std::size_t malloc_breakpoint_id;
    std::size_t free_breakpoint_id;
    std::size_t realloc_breakpoint_id;
public:
    ProgramManager(Misc::SmartPointer<Platform::ArgumentList> argument_list);
    virtual ~ProgramManager() {}
    
    void execute();
    void wait();
    
    /* called from a PTrace signal observer when the first SIGTRAP is caught, e.g. the child is loaded into memory */
    void place_initial_breakpoints();
    
    Misc::SmartPointer<PTrace::Portal> get_ptrace_portal() const { return ptrace_portal; }
    Misc::SmartPointer<ELF::Parser> get_elf_parser() const { return elf_parser; }
    Misc::SmartPointer<ELF::Parser> get_libc_parser() const { return libc_parser; }
    
    Misc::SmartPointer<Platform::ArgumentList> get_argument_list() const { return argument_list; }
    
    void set_running(bool new_running) { running = new_running; }
    bool is_running() const { return running; }
    
    std::size_t get_malloc_breakpoint_id() const { return malloc_breakpoint_id; }
    std::size_t get_free_breakpoint_id() const { return free_breakpoint_id; }
    std::size_t get_realloc_breakpoint_id() const { return realloc_breakpoint_id; }    
};

} // namespace Monitor
} // namespace Aesalon

#endif
