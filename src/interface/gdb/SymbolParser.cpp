#include <sstream>
#include <iomanip>
#include <iostream>

#include "SymbolParser.h"
#include "Controller.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

SymbolParser::SymbolParser(Misc::SmartPointer<Controller> gdb_controller) : gdb_controller(gdb_controller) {
    assembly_parser = new AssemblyParser();
    previous_stream_handler = gdb_controller->get_stream_handler();
    gdb_controller->set_stream_handler(this);
    in_scope = true;
    first = true;
}

SymbolParser::~SymbolParser() {
    gdb_controller->set_stream_handler(previous_stream_handler);
}

void SymbolParser::parse_symbol(Misc::SmartPointer<Platform::Symbol> symbol) {
    if(symbol->is_parsed()) return;
    current_symbol = symbol;
    first = true;
    in_scope = true; /* it's assumed that the first instruction is within the scope. */
    gdb_controller->send_command(Misc::StreamAsString() << "-interpreter-exec console \"x/i " << symbol->get_address() << "\"");
    while(true) {
        was_stream = false;
        while(!was_stream) gdb_controller->listen();
        if(in_scope) gdb_controller->send_command(Misc::StreamAsString() << "-interpreter-exec console \"x/i\"");
        else break;
    }
    
    symbol->set_parsed(true);
}

bool SymbolParser::handle(Misc::SmartPointer<String> string) {
    Misc::SmartPointer<StreamOutput> stream = string.to<StreamOutput>();
    std::cout << "SymbolParser::handle_stream: stream content is: " << stream->get_stream_data() << std::endl;
    was_stream = true;
    if(!in_scope) return true;
    if(first) {
        first = false;
        std::stringstream ss;
        ss << stream->get_stream_data();
        ss >> address;
        std::string symbol_name;
        ss >> symbol_name;
        scope = symbol_name.substr(1, symbol_name.length()-3);
        /*std::cout << "Scope is: \"" << scope << "\"\n";*/
    }
    else {
        std::stringstream ss;
        ss << stream->get_stream_data();
        ss >> address;
        std::string symbol_name;
        ss >> symbol_name;
        std::string temporary_scope;
        temporary_scope = symbol_name.substr(1, symbol_name.length()-3);
        temporary_scope = temporary_scope.substr(0, temporary_scope.find("+"));
        if(scope != temporary_scope) {
            /*std::cout << "Scope \"" << temporary_scope << "\" does not match original of \"" << scope << "\"" << std::endl;*/
            in_scope = false;
            return true;
        }
    }
    
    /* Add one to get rid of the tab that comes before the instruction . . . */
    std::string asm_instruction = stream->get_stream_data().substr(stream->get_stream_data().find(":")+2);
    asm_instruction.erase(asm_instruction.length()-1, 1);
    
    if(assembly_parser->changes_memory(asm_instruction)) add_breakpoint();
    return true;
}

void SymbolParser::add_breakpoint() {
    gdb_controller->send_command(Misc::StreamAsString() << "-break-insert *" << address << "\"");
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
