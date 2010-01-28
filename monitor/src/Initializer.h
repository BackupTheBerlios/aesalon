#ifndef AESALON_MONITOR_INITIALIZER_H
#define AESALON_MONITOR_INITIALIZER_H

#include "misc/SmartPointer.h"
#include "misc/Singleton.h"
#include "TCPServerSocket.h"
#include "platform/BidirectionalPipe.h"
#include "misc/EventQueue.h"
#include "ProgramManager.h"

namespace Aesalon {
namespace Monitor {

/** Initializer class. Basically handles initialization of the Monitor namespace. */
class Initializer : public Misc::Singleton<Initializer> {
private:
    /** A copy of argv, used for initializing the ArgumentParser. */
    char **argv;
    /** The socket used to communicate with the GUI. */
    Misc::SmartPointer<TCPServerSocket> server_socket;
    /** The EventQueue instance; used to keep the local copy of the program's memory updated. */
    Misc::SmartPointer<Misc::EventQueue> event_queue;
    
    Misc::SmartPointer<ProgramManager> program_manager;
    
    /** Initialize the aesalon monitor. */
    void initialize();
    /** Deinitialize the aesalon monitor. */
    void deinitialize();
    
    /** Print usage information about aesalon, including a list of all flags. */
    void usage();
    
    /** Main execution loop. */
    void run();
    
    int return_value;
public:
    /** Constructor for the Initializer class. This is where the magic begins.
        @param argv argv, as passed into main().
    */
    Initializer(char **argv);
    /** Destructor for the Initializer class. Here is where the magic ends. */
    virtual ~Initializer();
    
    /** Returns a SmartPointer to the named pipe; mostly used internally but
        public for other classes to use when required.
        @return The named pipe created by this aesalon gdb instance.
    */
    Misc::SmartPointer<TCPServerSocket> get_socket() const { return server_socket; }
    
    Misc::SmartPointer<ProgramManager> get_program_manager() const { return program_manager; }
    Misc::SmartPointer<Misc::EventQueue> get_event_queue() const { return event_queue; }
    
    int get_return_value() const { return return_value; }
    void set_return_value(int new_return_value) { return_value = new_return_value; }
};

} // namespace Monitor
} // namespace Aesalon

#endif
