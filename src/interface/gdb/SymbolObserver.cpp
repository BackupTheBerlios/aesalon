#include "SymbolObserver.h"
#include "interface/Initializer.h"
#include "misc/String.h"
#include "platform/MemoryAddress.h"

#include <sstream>

namespace Aesalon {
namespace Interface {
namespace GDB {

SymbolObserver::SymbolObserver() : StringObserver(String::STREAM_OUTPUT), finished_parsing(false) {
}

SymbolObserver::~SymbolObserver() {
}

bool SymbolObserver::notify(Misc::SmartPointer<String> string, Misc::SmartPointer<StateManager> state_manager) {
    if(state_manager->get_state() != StateManager::SETUP) return false;
    Misc::SmartPointer<StreamOutput> stream = string.to<StreamOutput>();
    
    std::string stream_data = stream->get_stream_data();
    std::cout << "stream_data: " << stream_data << std::endl;
    
    if(Misc::String::begins_with(stream_data, "\"0x")) {
        stream_data.erase(0, 1);
        std::stringstream ss;
        ss << stream_data;
        
        Platform::MemoryAddress symbol_address;
        ss >> std::hex >> symbol_address;
        
        if(!symbol_address) {
            return false;
        }
        
        std::string symbol_name;
        ss >> symbol_name;
        if(symbol_name[0] != '<') return false;
        symbol_name.erase(0, 1);
        symbol_name.erase(symbol_name.length()-2, 2);
        
        std::cout << "Symbol name: " << symbol_name << std::endl;
        if(symbol_name.find("+") == std::string::npos) {
            std::cout << "New scope found: \"" << symbol_name << "\"\n";
            if(current_scope != "") {
                parsed_scopes.push_back(current_scope);
            }
            current_scope = symbol_name;
        }
        
    }
    else if(Misc::String::begins_with(stream_data, "\"Cannot access memory at address 0x")) {
        /* All the symbols have been parsed now. */
        finished_parsing = true;
    }
    else {
        std::cout << "Skipping stream \"" << stream_data << "\"\n";
    }
    
    return false;
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
