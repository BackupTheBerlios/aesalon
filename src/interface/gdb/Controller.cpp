#include "Controller.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

Controller::Controller(Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe,
    Misc::SmartPointer<Misc::EventQueue> event_queue) : bi_pipe(bi_pipe), event_queue(event_queue) {
    processor = new Processor(bi_pipe, event_queue);
    send_command("-exec-run");
}

Controller::~Controller() {
    
}

void Controller::listen() {
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
