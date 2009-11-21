#include "Controller.h"
#include "String.h"
#include "misc/String.h"
#include "SetupManager.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

Controller::Controller(Misc::SmartPointer<Platform::BidirectionalPipe> gdb_pipe,
    Misc::SmartPointer<Misc::EventQueue> event_queue, Misc::SmartPointer<Platform::SymbolManager> symbol_manager)
    : gdb_pipe(gdb_pipe), event_queue(event_queue), symbol_manager(symbol_manager) {
    
    set_state(GDB_SETUP);
    
    /* Get rid of the introduction message. */
    sleep(1);
    listen();
    
    gdb_parser = new Parser();
    
    set_string_manager(new SetupManager());
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
    
    if(string_manager.is_valid()) string_manager->handle(string);
}

void Controller::send_command(std::string line) {
    if(get_state() == GDB_RUNNING) throw Misc::Exception("Attempt to send GDB command while program is running.");
    gdb_pipe->send_string(line + "\n");
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
