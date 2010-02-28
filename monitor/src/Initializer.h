#ifndef AESALON_MONITOR_INITIALIZER_H
#define AESALON_MONITOR_INITIALIZER_H

#include "misc/Singleton.h"
#include "tcp/ServerSocket.h"
#include "event/Queue.h"
#include "ProgramManager.h"
#include "misc/ArgumentParser.h"
#include "StorageManager.h"
#include "misc/ConfigParser.h"

/** Initializer class. Basically, handles initialization of the Monitor. */
class Initializer : public Misc::Singleton<Initializer> {
private:
    /** A copy of argv, used for initializing the ArgumentParser. */
    char **argv;
    /** The socket used to communicate with the GUI. */
    TCP::ServerSocket *server_socket;
    /** The EventQueue instance; used to keep the local copy of the program's memory updated. */
    Event::Queue *event_queue;
    
    ProgramManager *program_manager;
    
    Misc::ArgumentParser *argument_parser;
    
    Misc::ConfigParser *config_parser;
    
    StorageManager *storage_manager;
    
    /** Initialize the aesalon monitor. */
    void initialize();
    /** Deinitialize the aesalon monitor. */
    void deinitialize();
    
    /** Print version information .*/
    void version();
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
    TCP::ServerSocket *get_socket() const { return server_socket; }
    
    ProgramManager *get_program_manager() const { return program_manager; }
    Event::Queue *get_event_queue() const { return event_queue; }
    Misc::ArgumentParser *get_argument_parser() const { return argument_parser; }
    StorageManager *get_storage_manager() const { return storage_manager; }
    
    int get_return_value() const { return return_value; }
    void set_return_value(int new_return_value) { return_value = new_return_value; }
};

#endif
