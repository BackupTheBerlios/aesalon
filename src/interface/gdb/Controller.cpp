#include "Controller.h"
#include "SymbolParser.h"
#include "interface/SymbolManager.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

Controller::Controller(Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe,
    Misc::SmartPointer<Misc::EventQueue> event_queue, Misc::SmartPointer<SymbolManager> symbol_manager) :
    bi_pipe(bi_pipe), event_queue(event_queue), symbol_manager(symbol_manager) {
    
    processor = new Processor(bi_pipe, event_queue);
    this_sp = this;
    set_breakpoints();
    send_command("-gdb-set target-async 1");
    send_command("-exec-run");
    processor->set_gdb_state(Processor::GDB_RUNNING);
}

Controller::~Controller() {
}

void Controller::listen() {
    std::cout << "GDB state: " << get_state() << std::endl;
    if(get_state() == Processor::GDB_STOPPED) {
        static bool exit_sent = false; /* To prevent multiple -gdb-exits from being sent */
        if(!exit_sent) send_command("-gdb-exit");
        exit_sent = true;
    }
    std::string line;
    do {
        line = bi_pipe->get_string();
    
        processor->process(line);
    } while(line != "" && bi_pipe->is_open());
}

void Controller::send_command(std::string command) {
    std::cout << "GDB::Controller: sending command to gdb: \"" << command << "\"" << std::endl;
    bi_pipe->send_string(command + "\n");
}

void Controller::set_breakpoints() {
    symbol_parser = new SymbolParser(this_sp);
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
