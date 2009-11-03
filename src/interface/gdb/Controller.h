#ifndef AESALON_INTERFACE_GDB_CONTROLLER_H
#define AESALON_INTERFACE_GDB_CONTROLLER_H

#include <string>
#include "misc/EventQueue.h"
#include "Processor.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class Controller {
private:
    Misc::SmartPointer<Processor> processor;
    Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe;
    Misc::SmartPointer<Misc::EventQueue> event_queue;
    
public:
    Controller(Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe, Misc::SmartPointer<Misc::EventQueue> event_queue);
    virtual ~Controller();
    
    /* NOTE: send_command should be private eventually */
    void send_command(std::string command);
    void listen();
    
    Processor::gdb_state_e get_state() const { return processor->get_gdb_state(); }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
