#include "Controller.h"
#include "String.h"
#include "misc/String.h"
#include "SymbolObserver.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

Controller::Controller(Misc::SmartPointer<Platform::BidirectionalPipe> gdb_pipe,
    Misc::SmartPointer<Misc::EventQueue> event_queue, Misc::SmartPointer<Platform::SymbolManager> symbol_manager)
    : gdb_pipe(gdb_pipe), event_queue(event_queue), symbol_manager(symbol_manager) {
    
    observer_manager = new StringObserverManager();
    
    gdb_parser = new Parser();
    
    create_observers();
    
    set_state(StateManager::SETUP);
}

Controller::~Controller() {
}

void Controller::create_observers() {
    get_observer_manager()->add_observer(new SymbolObserver());
}

void Controller::listen() {
    if(get_state() == StateManager::SETUP) {
        process_symbols();
    }
    
    std::string line;
    do {
        line = gdb_pipe->get_string();
        process(line);
    } while(line != "");
}

void Controller::process(std::string line) {
    if(line.length() == 0) return;
    if(Misc::String::begins_with(line, "(gdb)")) {
        return;
    }
    
    Misc::SmartPointer<String> string = gdb_parser->parse_gdb_string(line);
    
    if(string.is_valid()) get_observer_manager()->notify(string, get_state_manager());
}

void Controller::send_command(std::string line) {
    if(get_state() == StateManager::RUNNING) throw Misc::Exception("Attempt to send GDB command while program is running.");
    gdb_pipe->send_string(line + "\n");
}

void Controller::process_symbols() {
    for(std::size_t x = 0; x < symbol_manager->get_symbols(); x ++) {
        Misc::SmartPointer<Platform::Symbol> symbol = symbol_manager->get_symbol(x);
        if(symbol->is_parsed()) continue;
        std::string gdb_command = Misc::StreamAsString() << "x/" << symbol->get_size() << "i " << symbol->get_address();
        std::cout << "Sending symbol command: " << gdb_command << std::endl;
        send_command(gdb_command);
        symbol->set_parsed(true);
    }
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
