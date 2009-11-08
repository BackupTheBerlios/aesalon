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
    
    if(!this->line.length()) {
        return;
    }
    if(begins_with("(gdb)")) {
        if(gdb_state != GDB_STOPPED) gdb_state = GDB_PAUSED;
        return;
    }
    
    Misc::SmartPointer<String> string = parser->parse_gdb_string(this->line);
    
    /* Now farm off the string as required . . . */
    
    if(string->get_type() == String::STREAM_OUTPUT) {
        if(stream_handler) {
            stream_handler->handle_stream(string.to<StreamOutput>());
        }
    }
    else if(string->get_type() == String::ASYNC_OUTPUT) {
        handle_async(string.to<AsyncOutput>());
    }
    else if(string->get_type() == String::RESULT_RECORD) handle_result(string.to<ResultRecord>());
    else throw Misc::Exception("Invalid String::string_type_e value");
}

void Processor::handle_async(Misc::SmartPointer<AsyncOutput> output) {
    if(output->get_data()->get_first() == "stopped") {
        if(output->get_data()->get_element("reason").is_valid()) {
            std::string reason = output->get_data()->get_element("reason").to<ParseResult>()->get_value().to<ParseString>()->get_data();
            
            set_gdb_state(GDB_PAUSED);
        }
    }
}

void Processor::handle_result(Misc::SmartPointer<ResultRecord> record) {
    if(record->get_data()->get_first() == "running") {
        set_gdb_state(GDB_RUNNING);
    }
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
