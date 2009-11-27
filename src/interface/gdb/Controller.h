#ifndef AESALON_INTERFACE_GDB_CONTROLLER_H
#define AESALON_INTERFACE_GDB_CONTROLLER_H

#include <string>

#include "Parser.h"
#include "platform/BidirectionalPipe.h"
#include "misc/EventQueue.h"
#include "platform/SymbolManager.h"
#include "StateManager.h"
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
    
    Misc::SmartPointer<StateManager> state_manager;
    
    void create_observers();
    void process_symbols();
public:
    Controller(Misc::SmartPointer<Platform::BidirectionalPipe> gdb_pipe,
        Misc::SmartPointer<Misc::EventQueue> event_queue, Misc::SmartPointer<Platform::SymbolManager> symbol_manager);
    virtual ~Controller();
    
    void send_command(std::string command);
    
    void listen();
    void process(std::string line);
    
    Misc::SmartPointer<StringObserverManager> get_observer_manager() const
        { return observer_manager; }
    
    StateManager::gdb_state_e get_state() const { return state_manager->get_state(); }
    void set_state(StateManager::gdb_state_e new_state) { state_manager->set_state(new_state); }
    
    Misc::SmartPointer<StateManager> get_state_manager() const { return state_manager; }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
