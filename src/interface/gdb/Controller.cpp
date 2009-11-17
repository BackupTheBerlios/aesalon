#include "Controller.h"
#include "String.h"
#include "misc/String.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

Controller::Controller(Misc::SmartPointer<Platform::BidirectionalPipe> gdb_pipe,
    Misc::SmartPointer<Misc::EventQueue> event_queue, Misc::SmartPointer<Platform::SymbolManager> symbol_manager)
    : gdb_pipe(gdb_pipe), event_queue(event_queue), symbol_manager(symbol_manager) {
    
    sleep(1);
    listen();
    
    symbol_parser = new SymbolParser(this);
}

Controller::~Controller() {

}

void Controller::listen() {
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
    
    
}

void Controller::send_command(std::string line) {
    gdb_pipe->send_string(line + "\n");
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
