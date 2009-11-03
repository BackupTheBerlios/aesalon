#include <string>
#include <sstream>

#include "Processor.h"
#include "platform/MemoryEvent.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

Processor::Processor(Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe,
    Misc::SmartPointer<Misc::EventQueue> event_queue) : bi_pipe(bi_pipe), event_queue(event_queue) {
    
    parser = new Parser();
}

void Processor::process(std::string line) {
    this->line = line;
    
    if(!this->line.length()) return;
    if(begins_with("(gdb)")) {
        return;
    }
    
    Misc::SmartPointer<String> string = parser->parse_gdb_string(this->line);
    
    /* Now farm off the string as required . . . */
    
    /* Don't bother with StreamOutputs, they're simply messages, nothing too useful */
    if(string->get_type() == String::STREAM_OUTPUT) return;
    else if(string->get_type() == String::ASYNC_OUTPUT) {
        handle_async(string.to<AsyncOutput>());
    }
    else handle_result(string.to<ResultRecord>());
}

void Processor::handle_async(Misc::SmartPointer<AsyncOutput> output) {
    
}

void Processor::handle_result(Misc::SmartPointer<ResultRecord> record) {
    if(record->get_data()->get_first() == "running") std::cout << "Program is running" << std::endl;
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
