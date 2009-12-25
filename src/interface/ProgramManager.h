#ifndef AESALON_INTERFACE_PROGRAM_MANAGER_H
#define AESALON_INTERFACE_PROGRAM_MANAGER_H

#include "misc/SmartPointer.h"
#include "platform/BidirectionalPipe.h"
#include "platform/ArgumentList.h"
#include "ptrace/Portal.h"
#include "elf/Parser.h"
#include "dwarf/Parser.h"

namespace Aesalon {
namespace Interface {

class ProgramManager {
private:
    Misc::SmartPointer<Platform::ArgumentList> argument_list;
    
    bool running;
    
    Misc::SmartPointer<PTrace::Portal> ptrace_portal;
    
    Misc::SmartPointer<ELF::Parser> elf_parser;
    Misc::SmartPointer<DWARF::Parser> dwarf_parser;
public:
    ProgramManager(Misc::SmartPointer<Platform::ArgumentList> argument_list);
    virtual ~ProgramManager() {}
    
    void execute();
    void wait();
    
    Misc::SmartPointer<PTrace::Portal> get_ptrace_portal() const { return ptrace_portal; }
    
    Misc::SmartPointer<Platform::ArgumentList> get_argument_list() const { return argument_list; }
    
    void set_running(bool new_running) { running = new_running; }
    bool is_running() const { return running; }
};

} // namespace Interface
} // namespace Aesalon

#endif
