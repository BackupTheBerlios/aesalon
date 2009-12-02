#include "Controller.h"
#include "String.h"
#include "misc/String.h"
#include "misc/ArgumentParser.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

Controller::Controller(Misc::SmartPointer<Platform::BidirectionalPipe> gdb_pipe,
    Misc::SmartPointer<Misc::EventQueue> event_queue, Misc::SmartPointer<Platform::SymbolManager> symbol_manager)
    : gdb_pipe(gdb_pipe), event_queue(event_queue), symbol_manager(symbol_manager) {
    
    observer_manager = new StringObserverManager();
    
    gdb_parser = new Parser();
    
    state_manager = new StateManager();
    
    add_observers();
    
    std::string program_arguments;
    for(std::size_t x = 1; x < Misc::ArgumentParser::get_instance()->get_files(); x ++) {
        program_arguments += Misc::ArgumentParser::get_instance()->get_file(x)->get_filename();
        program_arguments += " ";
    }
    send_command("-exec-arguments " + program_arguments);
    
    send_command("-break-insert _start");
    send_command("-exec-run");
}

Controller::~Controller() {
}

void Controller::add_observers() {
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
    
    if(string.is_valid()) get_observer_manager()->notify(string);
}

void Controller::send_command(std::string line) {
    if(get_state() == State::RUNNING) throw Misc::Exception("Attempt to send GDB command while program is running.");
    std::cout << "Sending command to gdb: \"" << line << "\"\n";
    gdb_pipe->send_string(line + "\n");
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
