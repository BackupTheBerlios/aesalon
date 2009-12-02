#ifndef AESALON_INTERFACE_GDB_CONTROLLER_H
#define AESALON_INTERFACE_GDB_CONTROLLER_H

#include <string>

#include "Parser.h"
#include "platform/BidirectionalPipe.h"
#include "misc/EventQueue.h"
#include "platform/SymbolManager.h"
#include "StringObserverManager.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class Controller {
private:
    Misc::SmartPointer<Platform::BidirectionalPipe> gdb_pipe;
    Misc::SmartPointer<Parser> gdb_parser;
    Misc::SmartPointer<Misc::EventQueue> event_queue;
    Misc::SmartPointer<StringObserverManager> observer_manager;
    Misc::SmartPointer<Platform::SymbolManager> symbol_manager;
    bool running;
    
    void add_observers();
public:
    Controller(Misc::SmartPointer<Platform::BidirectionalPipe> gdb_pipe,
        Misc::SmartPointer<Misc::EventQueue> event_queue, Misc::SmartPointer<Platform::SymbolManager> symbol_manager);
    virtual ~Controller();
    
    void send_command(std::string command);
    
    void listen();
    void process(std::string line);
    
    Misc::SmartPointer<StringObserverManager> get_observer_manager() const
        { return observer_manager; }
    
    bool is_running() const { return running; }
    void set_running(bool new_running) { running = new_running; }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
