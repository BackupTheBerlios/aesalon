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
        std::cout << "\tProgram execution has stopped for some reason . . ." << std::endl;
        if(StringFollower(async).follow("'reason' rhs") != "breakpoint-hit") {
            std::cout << "\t . . . wasn't a breakpoint-hit . . ." << std::endl;
            return false;
        }
        if(StringFollower(async).follow("'bkptno' rhs") != "1") {
            std::cout << "\t . . . the breakpoint number wasn't 1 . . ." << std::endl;
            return false;
        }
        
        request_next();
        
        return true;
    }
    return false;
}

bool SymbolObserver::notify(Misc::SmartPointer<ResultRecord> result) {
    std::cout << "SymbolObserver::notify(ResultRecord):" << std::endl;
    std::cout << "\tresult->get_data()->get_first(): " << result->get_data()->get_first() << std::endl;
    
    if(result->get_data()->get_first() == "error") {
        if(Misc::String::begins_with(StringFollower(result).follow("'msg' rhs"), "Cannot access memory at address 0x")) {
            set_alive(false);
            return true;
        }
    }
    
    return false;
}

bool SymbolObserver::notify(Misc::SmartPointer<StreamOutput> stream) {
    std::string data = stream->get_stream_data();
    data = Misc::String::remove_escapes(data);
    data = data.substr(1, data.length()-2);
    std::cout << "SymbolObserver::notify(StreamOutput): data is \"" << data << "\"\n";
    
    if(Misc::String::begins_with(data, "0x")) {
        instructions_parsed ++;
        if(instructions_parsed == INSTRUCTION_BLOCK_SIZE) {
            instructions_parsed = 0;
            request_next();
        }
        Platform::MemoryAddress address;
        std::istringstream parser(data.substr(0, data.find(" ")));
        parser >> std::hex >> address;
        last_address = address;
    }
    
    return false;
}

void SymbolObserver::request_next() {
    if(!last_address) last_address = symbol_manager->get_global(0)->get_address();
    Initializer::get_instance()->get_controller()->send_command(
        Misc::StreamAsString() << "-interpreter-exec console \"x/" << INSTRUCTION_BLOCK_SIZE << "i " << last_address << "\"");
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
