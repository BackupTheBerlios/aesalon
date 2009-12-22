#ifndef AESALON_GUI_PROGRAM_H
#define AESALON_GUI_PROGRAM_H

#include "platform/EventQueue.h"
#include "platform/TCPSocket.h"
#include "platform/Memory.h"
#include "platform/BidirectionalPipe.h"

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace GUI {

class Program {
private:
    Misc::SmartPointer<Platform::EventQueue> event_queue;
    Misc::SmartPointer<Platform::Memory> memory;
    Misc::SmartPointer<Platform::TCPSocket> socket;
    
    std::string executable;
    std::string arguments;
    int port;
    bool in_xterm;
    
    Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe;
public:
    Program();
    virtual ~Program() {}
    
    std::string get_executable() const { return executable; }
    std::string get_arguments() const { return arguments; }
    bool is_in_xterm() const { return in_xterm; }
    
    bool is_running() const { if(bi_pipe.is_valid()) return bi_pipe->is_open(); return false; }
};

} // namespace GUI
} // namespace Aesalon

#endif
