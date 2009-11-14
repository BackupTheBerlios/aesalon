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
    
    processor->set_gdb_state(Processor::GDB_SETUP);
    
    sleep(1);
    this->listen(true);
    
    processor->set_gdb_state(Processor::GDB_SETUP);
    
    /*send_command("-gdb-set target-async 1");*/
    send_command("-break-insert main");
    send_command("-exec-run");
    
    sleep(1);
    this->listen(true);
    
    processor->set_gdb_state(Processor::GDB_SETUP);
    
    
    /*processor->set_gdb_state(Processor::GDB_SETUP);*/
    set_breakpoints();
    
    std::cout << "Breakpoints have been set, continuing execution . . ." << std::endl;
    send_command("-break-delete 1"); /* Delete the breakpoint on main, as it is no longer required */
    /*send_command("-exec-continue");*/
    processor->set_gdb_state(Processor::GDB_PAUSED);
}

Controller::~Controller() {
}

void Controller::listen(bool wait) {
    static bool exit_sent = false; /* To prevent multiple -gdb-exits from being sent */
    static bool update_sent = false; /* To prevent multiple -var-updates from being sent */
    
    if(get_state() == Processor::GDB_STOPPED) {
        if(!exit_sent) send_command("-gdb-exit");
        exit_sent = true;
    }
    
    /* line = bi_pipe->get_string();
            if(line != "") processor->process(line); */
    
    std::string line;
    
    
    bool recved = false;
    do {
        line = bi_pipe->get_string();
        if(line != "") {
            processor->process(line);
            std::cout << "GDB::Controller: received string \'" << line << "\'\n";
            recved = true;
        }
    } while(bi_pipe->is_open() && ((wait && !recved) || (!wait && line != "")));
    
    if(get_state() == Processor::GDB_PAUSED && !update_sent) {
        /*send_command("-var-update --all-values \"*\"");*/
        send_command("-exec-continue");
        update_sent = true;
    }
    else if(get_state() == Processor::GDB_RUNNING) update_sent = false;
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
        Misc::SmartPointer<Symbol> symbol = symbol_manager->get_symbol(x);
        symbol_parser->parse_symbol(symbol);
    }
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
