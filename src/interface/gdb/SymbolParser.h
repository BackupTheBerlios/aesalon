#ifndef AESALON_INTERFACE_GDB_SYMBOL_PARSER_H
#define AESALON_INTERFACE_GDB_SYMBOL_PARSER_H

#include "AssemblyParser.h"
#include "misc/SmartPointer.h"
#include "platform/Symbol.h"
#include "StreamHandler.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class Controller;

class SymbolParser : public StreamHandler {
private:
    Misc::SmartPointer<AssemblyParser> assembly_parser;
    Misc::SmartPointer<StreamHandler> previous_stream_handler;
    
    Misc::SmartPointer<Platform::Symbol> current_symbol;
    
    bool in_scope;
    std::string scope;
    std::string address;
    bool first;
    bool was_stream;
    
    void add_breakpoint();
public:
    SymbolParser(Misc::SmartPointer<Controller> controller);
    virtual ~SymbolParser();
    
    void parse_symbol(Misc::SmartPointer<Platform::Symbol> symbol);
    
    virtual bool handle(Misc::SmartPointer<StreamOutput> stream);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
