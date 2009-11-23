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
    argument_list.add_argument(Misc::StreamAsString() << "--demangle" << (demangle_style != "" ? "=" + demangle_style : ""));
    argument_list.add_argument("--format=bsd");
    argument_list.add_argument(executable_name);
    bi_pipe = new Platform::BidirectionalPipe(nm_path, argument_list, true);
    parse_nm_output();
}

void SymbolManager::parse_nm_output() {
    std::string line;
    MemoryAddress fini_address = 0;
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
                /* Also skip the special symbol data_start. */
                if(symbol_name != "_init" && symbol_name != "_start" && symbol_name != "_fini" && symbol_name != "data_start") 
                    symbol_list.push_back(new Symbol(symbol_address, 0, Symbol::FUNCTION, symbol_name));
                
                /* . . . but save the address of _fini. */
                if(symbol_name == "_fini") fini_address = symbol_address;
                break;
            /*case 'b':*/
            case 'B':
                global_list.push_back(new Symbol(symbol_address, 0, Symbol::FUNCTION, symbol_name));
                break;
            default: break;
        }
        
    } while(bi_pipe->is_open());
    
    if(!symbol_list.size()) return;
    /* Now, go though and calculate sizes. */
    symbol_vector_t::iterator i = symbol_list.begin();
    for(; i != symbol_list.end() && (i+1) != symbol_list.end(); i ++) {
        (*i)->set_size((*(i+1))->get_address() - (*i)->get_address());
    }
    (*i+1)->set_size(fini_address - (*i)->get_address());
}

} // namespace Platform
} // namespace Aesalon
