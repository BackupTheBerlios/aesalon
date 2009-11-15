#include "SymbolManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>

namespace Aesalon {
namespace Platform {

SymbolManager::SymbolManager() : nm_path("/usr/bin/nm"), demangle_style("") {

}

SymbolManager::~SymbolManager() {

}

void SymbolManager::parse_from_executable(std::string executable_name) {
    this->executable_name = executable_name;
    
    setup_pipe();
}

void SymbolManager::setup_pipe() {
    Platform::ArgumentList argument_list;
    argument_list.add_argument(nm_path);
    argument_list.add_argument("--numeric-sort");
    argument_list.add_argument("--defined-only");
    argument_list.add_argument(Misc::StreamAsString() << "--demangle" << (demangle_style != "" ? "="+demangle_style : ""));
    argument_list.add_argument("--format=bsd");
    argument_list.add_argument(executable_name);
    bi_pipe = new Platform::BidirectionalPipe(nm_path, argument_list, true);
    parse_nm_output();
}

void SymbolManager::parse_nm_output() {
    std::string line;
    do {
        std::stringstream line_stream;
        
        line = bi_pipe->get_string();
        
        line_stream << line;
        
        line_stream >> std::hex >> symbol_address >> std::dec >> symbol_type >> symbol_name;
        
        switch(symbol_type) {
            /*case 't':*/
            case 'T':
            /*case 'w':*/
            case 'W':
                /* Skip the three ELF symbols, as they do not allocate memory we are interested in . . . */
                if(symbol_name == "_init" || symbol_name == "_start" || symbol_name == "_fini") break;
                symbol_list.push_back(new Symbol(symbol_address, Symbol::FUNCTION, symbol_name));
                break;
            /*case 'b':*/
            case 'B':
                global_list.push_back(new Symbol(symbol_address, Symbol::FUNCTION, symbol_name));
                break;
            default: break;
        }
        
    } while(bi_pipe->is_open());
}

} // namespace Platform
} // namespace Aesalon
