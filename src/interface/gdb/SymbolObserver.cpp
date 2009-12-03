#include <iostream>
#include <sstream>

#include "SymbolObserver.h"
#include "StringFollower.h"
#include "interface/Initializer.h"
#include "misc/String.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

bool SymbolObserver::notify(Misc::SmartPointer<AsyncOutput> async) {
    if(async->get_data()->get_first() == "stopped") {
        if(StringFollower(async).follow("'reason' rhs") != "breakpoint-hit") return false;
        if(StringFollower(async).follow("'bkptno' rhs") != "1") return false;
        
        request_next();
        
        return true;
    }
    return false;
}

bool SymbolObserver::notify(Misc::SmartPointer<ResultRecord> result) {
    if(result->get_data()->get_first() == "error") {
        if(Misc::String::begins_with(StringFollower(result).follow("'msg' rhs"), "Cannot access memory at address 0x")) {
            set_alive(false);
            Initializer::get_instance()->get_controller()->send_command("-break-delete 1");
            Initializer::get_instance()->get_controller()->send_command("-exec-continue");
            return true;
        }
    }
    
    return false;
}

bool SymbolObserver::notify(Misc::SmartPointer<StreamOutput> stream) {
    std::string data = stream->get_stream_data();
    data = Misc::String::remove_escapes(data);
    data = data.substr(1, data.length()-2);
    
    if(Misc::String::begins_with(data, "0x")) {
        instructions_parsed ++;
        if(instructions_parsed == INSTRUCTION_BLOCK_SIZE) {
            instructions_parsed = 0;
            request_next();
        }
        Platform::MemoryAddress address;
        Misc::String::to<Platform::MemoryAddress>(data.substr(0, data.find(" ")), address, true);
        last_address = address;
        
        std::string asm_line = data.substr(data.find(":")+2);
        if(assembly_parser->changes_memory(asm_line)) add_breakpoint();
    }
    
    return false;
}

void SymbolObserver::request_next() {
    if(!last_address) last_address = symbol_manager->get_symbol(0)->get_address();
    Initializer::get_instance()->get_controller()->send_command(
        Misc::StreamAsString() << "-interpreter-exec console \"x/" << INSTRUCTION_BLOCK_SIZE << "i " << last_address << "\"");
}

void SymbolObserver::add_breakpoint() {
    Initializer::get_instance()->get_controller()->send_command(Misc::StreamAsString() << "-break-insert *" << last_address);
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
