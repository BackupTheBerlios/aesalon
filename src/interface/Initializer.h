#ifndef AESALON_INTERFACE_INITIALIZER_H
#define AESALON_INTERFACE_INITIALIZER_H

#include "misc/SmartPointer.h"
#include "misc/Singleton.h"
#include "platform/NamedPipe.h"
#include "platform/BidirectionalPipe.h"
#include "gdb/Controller.h"
#include "misc/EventQueue.h"

#include "platform/SymbolManager.h"

namespace Aesalon {
namespace Interface {

/** Initializer class. Basically handles initialization of the Interface namespace. */
class Initializer : public Misc::Singleton<Initializer> {
private:
    /** A copy of argv, used for initializing the ArgumentParser. */
    char **argv;
    /** The named pipe used to communicate with the GUI. */
    Misc::SmartPointer<Platform::NamedPipe> named_pipe;
    /** The BidirectionalPipe instance used to communicate with gdb. */
    Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe;
    /** The GDB Controller instance; handles all gdb input and output. */
    Misc::SmartPointer<Interface::GDB::Controller> gdb_controller;
    /** The EventQueue instance; used to keep the local copy of the program's memory updated. */
    Misc::SmartPointer<Misc::EventQueue> event_queue;
    
    Misc::SmartPointer<Platform::SymbolManager> symbol_manager;
    
    /** Initialize the aeslaon gdb interface. */
    void initialize();
    /** Deinitialize the aesalon gdb interface. */
    void deinitialize();
    /** Send the PID of the current gdb interface along to a pre-existing GUI instance. */
    void send_pid_to_gui();
    
    /** Print usage information about aesalon, including a list of all flags. */
    void usage();
    
    /** Main execution loop. */
    void run();
public:
    /** Constructor for the Initializer class. This is where the magic begins.
        @arg argv argv, as passed into main().
    */
    Initializer(char **argv);
    /** Destructor for the Initializer class. Here is where the magic ends. */
    virtual ~Initializer();
    
    /** Returns a SmartPointer to the named pipe; mostly used internally but
        public for other classes to use when required.
        @return The named pipe created by this aesalon gdb instance.
    */
    Misc::SmartPointer<Platform::NamedPipe> get_named_pipe() const { return named_pipe; }
    Misc::SmartPointer<GDB::Controller> get_controller() const { return gdb_controller; }
};

} // namespace Interface
} // namespace Aesalon

#endif
