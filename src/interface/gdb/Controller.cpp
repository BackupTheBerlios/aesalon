#include "Controller.h"
#include "String.h"
#include "misc/String.h"
#include "misc/ArgumentParser.h"
#include "SymbolObserver.h"
#include "ExitObserver.h"
#include "BreakpointObserver.h"
#include "BreakpointSetupObserver.h"
#include "MallocObserver.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

Controller::Controller(Misc::SmartPointer<Platform::BidirectionalPipe> gdb_pipe,
    Misc::SmartPointer<Platform::EventQueue> event_queue, Misc::SmartPointer<Platform::SymbolManager> symbol_manager)
    : gdb_pipe(gdb_pipe), event_queue(event_queue), symbol_manager(symbol_manager), running(true) {
    
    observer_manager = new StringObserverManager();
    
    gdb_parser = new Parser();
    
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
    /* Add setup observers first . . . */
    get_observer_manager()->add_observer(new SymbolObserver(symbol_manager));
    get_observer_manager()->add_temporary_observer(new BreakpointSetupObserver());
    
    /* Now for the regular observers . . . */
    get_observer_manager()->add_observer(new BreakpointObserver());
    get_observer_manager()->add_observer(new ExitObserver());
    get_observer_manager()->add_observer(new MallocObserver(event_queue));
}

void Controller::listen() {
    std::string line;
    
    get_observer_manager()->cleanup();
    
    do {
        line = gdb_pipe->get_string();
        process(line);
    } while(line != "");
}

void Controller::process(std::string line) {
    if(line.length() == 0) return;
    std::cout << "Received line from gdb: " << line << std::endl;
    if(Misc::String::begins_with(line, "(gdb)")) {
        return;
    }
    
    Misc::SmartPointer<String> string = gdb_parser->parse_gdb_string(line);
    
    if(string.is_valid()) get_observer_manager()->notify(string);
}

void Controller::send_command(std::string line) {
    std::cout << "Sending command to gdb: \"" << line << "\"\n";
    gdb_pipe->send_string(line + "\n");
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
