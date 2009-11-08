#ifndef AESALON_INTERFACE_GDB_SYMBOL_PARSER_H
#define AESALON_INTERFACE_GDB_SYMBOL_PARSER_H

#include "AssemblyParser.h"
#include "misc/SmartPointer.h"
#include "interface/Symbol.h"
#include "StreamHandler.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class Controller;

class SymbolParser : public StreamHandler {
private:
    Misc::SmartPointer<AssemblyParser> assembly_parser;
    Misc::SmartPointer<Controller> gdb_controller;
    Misc::SmartPointer<StreamHandler> previous_stream_handler;
    
    Misc::SmartPointer<Symbol> current_symbol;
    
    bool in_scope;
    std::string scope;
    bool first;
    bool was_stream;
    
    void add_breakpoint();
public:
    SymbolParser(Misc::SmartPointer<Controller> gdb_controller);
    virtual ~SymbolParser();
    
    void parse_symbol(Misc::SmartPointer<Symbol> symbol);
    
    void handle_stream(Misc::SmartPointer<StreamOutput> stream);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
