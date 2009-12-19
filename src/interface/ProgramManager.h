#ifndef AESALON_INTERFACE_PROGRAM_MANAGER_H
#define AESALON_INTERFACE_PROGRAM_MANAGER_H

#include "misc/SmartPointer.h"
#include "platform/BidirectionalPipe.h"
#include "platform/ArgumentList.h"
#include "PTracePortal.h"
#include "elf/Parser.h"

namespace Aesalon {
namespace Interface {

class ProgramManager {
private:
    Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe;
    Misc::SmartPointer<Platform::ArgumentList> argument_list;
    
    bool running;
    
    Misc::SmartPointer<PTracePortal> ptrace_portal;
    
    Misc::SmartPointer<ELF::Parser> elf_parser;
public:
    ProgramManager(Misc::SmartPointer<Platform::ArgumentList> argument_list);
    virtual ~ProgramManager() {}
    
    void execute();
    void wait();
    void handle();
    
    Misc::SmartPointer<Platform::ArgumentList> get_argument_list() const { return argument_list; }
    
    bool is_running() const { return running; }
};

} // namespace Interface
} // namespace Aesalon

#endif
