#include <sstream>

#include "SymbolParser.h"
#include "Controller.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

SymbolParser::SymbolParser(Misc::SmartPointer<Controller> gdb_controller) : gdb_controller(gdb_controller) {
    assembly_parser = new AssemblyParser();
    previous_stream_handler = gdb_controller->get_processor()->get_stream_handler();
    gdb_controller->get_processor()->set_stream_handler(this);
    in_scope = true;
    first = true;
}

SymbolParser::~SymbolParser() {
    gdb_controller->get_processor()->set_stream_handler(previous_stream_handler);
}

void SymbolParser::parse_symbol(Misc::SmartPointer<Symbol> symbol) {
    if(symbol->is_parsed()) return;
    current_symbol = symbol;
    first = true;
    gdb_controller->send_command(Misc::StreamAsString() << "-interpreter-exec console \"x/i " << symbol->get_address() << "\"");
    gdb_controller->listen();
    while(in_scope) {
        gdb_controller->send_command(Misc::StreamAsString() << "-interpreter-exec console \"x/i\"");
        gdb_controller->listen();
    }
    
    symbol->set_parsed(true);
}

void SymbolParser::handle_stream(Misc::SmartPointer<StreamOutput> stream) {
    if(first) {
        first = false;
        std::stringstream ss;
        ss << stream->get_stream_data();
        std::string address;
        ss >> address;
        std::string symbol_name;
        ss >> symbol_name;
        scope = symbol_name.substr(1, symbol_name.length()-3);
        std::cout << "Scope is: \"" << scope << "\"\n";
    }
    else {
        std::stringstream ss;
        ss << stream->get_stream_data();
        std::string address;
        ss >> address;
        std::string symbol_name;
        ss >> symbol_name;
        std::string temporary_scope;
        temporary_scope = symbol_name.substr(1, symbol_name.length()-3);
        temporary_scope = temporary_scope.substr(0, temporary_scope.find("+"));
        if(scope != temporary_scope) {
            std::cout << "Scope \"" << temporary_scope << "\" does not match original of \"" << scope << "\"" << std::endl;
            in_scope = false;
            return;
        }
    }
    
    /* Add one to get rid of the tab that comes before the instruction . . . */
    std::string asm_instruction = stream->get_stream_data().substr(stream->get_stream_data().find(":")+2);
    asm_instruction.erase(asm_instruction.length()-1, 1);
    
    if(assembly_parser->changes_memory(asm_instruction)) add_breakpoint();
    
}

void SymbolParser::add_breakpoint() {
    
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
