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
    
    processor->set_gdb_state(Processor::GDB_PAUSED);
    
    /* NOTE: !!! For testing purposes only! */
    this->listen(true);
    sleep(1);
    for(int x = 0; x < 25; x ++) {
        std::cout << "Skipping GDB intro message . . ." << std::endl;
        this->listen(false);
    }
    
    set_breakpoints();
    
    send_command("-gdb-set target-async 1");
    send_command("-exec-run");
    processor->set_gdb_state(Processor::GDB_RUNNING);
}

Controller::~Controller() {
}

void Controller::listen(bool wait) {
    std::cout << "GDB state: " << get_state() << std::endl;
    if(get_state() == Processor::GDB_STOPPED) {
        static bool exit_sent = false; /* To prevent multiple -gdb-exits from being sent */
        if(!exit_sent) send_command("-gdb-exit");
        exit_sent = true;
    }
    std::string line;
    do {
        line = bi_pipe->get_string();
        if(line != "") processor->process(line);
    } while(wait && line != "" && bi_pipe->is_open());
}

void Controller::send_command(std::string command) {
    std::cout << "GDB::Controller: sending command to gdb: \"" << command << "\"" << std::endl;
    bi_pipe->send_string(command + "\n");
}

void Controller::set_breakpoints() {
    std::cout << "Setting breakpoints . . ." << std::endl;
    
    symbol_parser = new SymbolParser(this_sp);
    
    SymbolManager::symbol_vector_t::size_type x;
    for(x = 0; x < symbol_manager->get_symbols(); x ++) {
        std::cout << "Calling symbol_parser->parse_symbol() . . ." << std::endl;
        Misc::SmartPointer<Symbol> symbol = symbol_manager->get_symbol(x);
        symbol_parser->parse_symbol(symbol);
    }
    
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
