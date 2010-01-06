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
    
    Misc::SmartPointer<ELF::Parser> elf_parser;
    Misc::SmartPointer<DWARF::Parser> dwarf_parser;
    Misc::SmartPointer<ASM::Disassembler> disassembler;
public:
    ProgramManager(Misc::SmartPointer<Platform::ArgumentList> argument_list);
    virtual ~ProgramManager() {}
    
    void execute();
    void wait();
    
    Misc::SmartPointer<PTrace::Portal> get_ptrace_portal() const { return ptrace_portal; }
    Misc::SmartPointer<ELF::Parser> get_elf_parser() const { return elf_parser; }
    
    Misc::SmartPointer<Platform::ArgumentList> get_argument_list() const { return argument_list; }
    
    void set_running(bool new_running) { running = new_running; }
    bool is_running() const { return running; }
};

} // namespace Monitor
} // namespace Aesalon

#endif
