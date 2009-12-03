#ifndef AESALON_GUI_PROGRAM_H
#define AESALON_GUI_PROGRAM_H

#include "platform/EventQueue.h"
#include "platform/NamedPipe.h"
#include "platform/Memory.h"
#include "platform/BidirectionalPipe.h"

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace GUI {

class Program {
private:
    Misc::SmartPointer<Platform::EventQueue> event_queue;
    Misc::SmartPointer<Platform::Memory> memory;
    Misc::SmartPointer<Platform::NamedPipe> named_pipe;
    
    std::string executable;
    std::string arguments;
    bool in_xterm;
    
    Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe;
public:
    Program(std::string executable, std::string arguments, bool in_xterm);
    virtual ~Program() {}
    
    std::string get_executable() const { return executable; }
    std::string get_arguments() const { return arguments; }
    bool is_in_xterm() const { return in_xterm; }
};

} // namespace GUI
} // namespace Aesalon

#endif
