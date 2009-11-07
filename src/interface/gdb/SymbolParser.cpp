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
    gdb_controller->send_command(Misc::StreamAsString() << "-interpreter-exec console \"x/i " << symbol->get_address() << "\"");
    gdb_controller->listen();
    while(in_scope) {
        gdb_controller->send_command(Misc::StreamAsString() << "-interpreter-exec console \"x/i\"");
        gdb_controller->listen();
        usleep(50000);
    }
    
    symbol->set_parsed(true);
}

void SymbolParser::handle_stream(Misc::SmartPointer<StreamOutput> stream) {
    std::cout << "Received stream \"" << stream->get_stream_data() << "\"" << std::endl;
    if(first) {
        first = false;
        std::stringstream ss;
        ss << stream;
        std::string address;
        ss >> address;
        std::string symbol_name;
        ss >> symbol_name;
        scope = symbol_name.substr(1, symbol_name.length()-3);
        std::cout << "Scope is: \"" << scope << "\"\n";
    }
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
