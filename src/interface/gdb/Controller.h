#ifndef AESALON_INTERFACE_GDB_CONTROLLER_H
#define AESALON_INTERFACE_GDB_CONTROLLER_H

#include <string>

#include "StringManager.h"
#include "Parser.h"
#include "platform/BidirectionalPipe.h"
#include "misc/EventQueue.h"
#include "platform/SymbolManager.h"
#include "SymbolParser.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class StringManager;

class Controller {
public:
    enum gdb_state_e {
        GDB_SETUP, /**< GDB is currently being set up, initialization is in progress. */
        GDB_RUNNING, /**< GDB is currently running. No commands can be executed during this state. */
        GDB_PAUSED, /**< GDB is paused, but has been running previously, and setup is complete. */
        GDB_FINISHED /**< GDB has finished executing the program. */
    };
private:
    Misc::SmartPointer<StringManager> string_manager;
    
    Misc::SmartPointer<Platform::BidirectionalPipe> gdb_pipe;
    Misc::SmartPointer<Parser> gdb_parser;
    Misc::SmartPointer<Misc::EventQueue> event_queue;
    Misc::SmartPointer<Platform::SymbolManager> symbol_manager;
    Misc::SmartPointer<SymbolParser> symbol_parser;
    
    gdb_state_e gdb_state;
public:
    Controller(Misc::SmartPointer<Platform::BidirectionalPipe> gdb_pipe,
        Misc::SmartPointer<Misc::EventQueue> event_queue, Misc::SmartPointer<Platform::SymbolManager> symbol_manager);
    virtual ~Controller();
    
    void send_command(std::string command);
    
    Misc::SmartPointer<StringManager> get_string_manager() const { return string_manager; }
    void set_string_manager(Misc::SmartPointer<StringManager> new_handler)
        { string_manager = new_handler; }
    
    void listen();
    void process(std::string line);
    
    gdb_state_e get_state() const { return gdb_state; }
    void set_state(gdb_state_e new_state) { gdb_state = new_state; }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
