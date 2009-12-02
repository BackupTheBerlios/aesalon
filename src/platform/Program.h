#ifndef AESALON_PLATFORM_PROGRAM_H
#define AESALON_PLATFORM_PROGRAM_H

#include <string>

#include "BidirectionalPipe.h"
#include "PipeListener.h"
#include "ArgumentList.h"
#include "Memory.h"
#include "misc/SmartPointer.h"
#include "EventQueue.h"

namespace Aesalon {
namespace Platform {
/* NOTE: forward declaration. */
class PipeListener;

class Program {
private:
    Misc::SmartPointer<BidirectionalPipe> gdb_pipe;
    Misc::SmartPointer<PipeListener> pipe_listener;
    Misc::SmartPointer<Memory> program_memory;
    ArgumentList argument_list;
    Misc::SmartPointer<EventQueue> event_queue;
public:
    Program();
    virtual ~Program();
    
    Misc::SmartPointer<Memory> get_program_memory() const { return program_memory; }
    
    void execute(std::string executable);
    
    Misc::SmartPointer<EventQueue> get_event_queue() const { return event_queue; }
};

} // namespace Platform
} // namespace Aesalon

#endif
