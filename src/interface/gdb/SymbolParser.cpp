#include "SymbolParser.h"
#include "Controller.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

SymbolParser::SymbolParser(Misc::SmartPointer<Controller> gdb_controller) : gdb_controller(gdb_controller) {
    assembly_parser = new AssemblyParser();
    previous_stream_handler = gdb_controller->get_processor()->get_stream_handler();
    gdb_controller->get_processor()->set_stream_handler(this);
}

SymbolParser::~SymbolParser() {
    gdb_controller->get_processor()->set_stream_handler(previous_stream_handler);
}

void SymbolParser::parse_symbol(Misc::SmartPointer<Symbol> symbol) {
    gdb_controller->send_command(Misc::StreamAsString() << "-interpreter-exec console \"x/2i " << symbol->get_address() << "\"");
}

void SymbolParser::handle_stream(Misc::SmartPointer<StreamOutput> stream) {
    std::cout << "Received stream \"" << stream->get_stream_data() << "\"" << std::endl;
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
