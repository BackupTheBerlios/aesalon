#include "Controller.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

Controller::Controller(Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe,
    Misc::SmartPointer<Misc::EventQueue> event_queue) : bi_pipe(bi_pipe), event_queue(event_queue) {
    processor = new Processor(bi_pipe, event_queue);
    send_command("-exec-run");
    processor->set_gdb_state(Processor::GDB_RUNNING);
}

Controller::~Controller() {
    
}

void Controller::listen() {
    std::cout << "GDB state: " << get_state() << std::endl;
    if(get_state() == Processor::GDB_STOPPED) {
        send_command("-gdb-exit");
    }
    std::string line = bi_pipe->get_string();
    
    processor->process(line);
}

void Controller::send_command(std::string command) {
    std::cout << "GDB::Controller: sending command to gdb: \"" << command << "\"" << std::endl;
    bi_pipe->send_string(command + "\n");
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
