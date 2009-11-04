#ifndef AESALON_INTERFACE_GDB_CONTROLLER_H
#define AESALON_INTERFACE_GDB_CONTROLLER_H

#include <string>
#include "misc/EventQueue.h"
#include "Processor.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

/** GDB Controller class; handles all input and output between GDB and aesalon. */
class Controller {
private:
    /** SmartPointer to the GDB Processor, which handles GDB output. */
    Misc::SmartPointer<Processor> processor;
    /** BidirectionalPipe instance used to communicate with GDB, saved for reference. */
    Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe;
    /** EventQueue to push MemoryEvents onto; saved for reference. */
    Misc::SmartPointer<Misc::EventQueue> event_queue;
    
public:
    /** Constructor for the GDB Controller.
        @param bi_pipe A BidirectionalPipe connected to a running instance of GDB.
        @param event_queue An EventQueue to push MemoryEvents onto. */
    Controller(Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe, Misc::SmartPointer<Misc::EventQueue> event_queue);
    /** Destructor for the GDB Controller. */
    virtual ~Controller();
    
    /** Sends a command along to GDB. Adds a newline on the end of the given string.
        @param command The command for GDB to execute.
        @note This method should eventually be private,
            but it will be used for debugging purposes as public until then.
    */
    void send_command(std::string command);
    /** Listen on the BidirectionalPipe for GDB output, then pass it onto the @a Processor instance.
    */
    void listen();
    
    /** Returns the state of the GDB processor.
        @return The state of the GDB processor.
        @note This method is a passthrough to Processor::get_state().
    */
    Processor::gdb_state_e get_state() const { return processor->get_gdb_state(); }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
