#ifndef AESALON_INTERFACE_GDB_CONTROLLER_H
#define AESALON_INTERFACE_GDB_CONTROLLER_H

#include <string>

#include "StringHandlerManager.h"
#include "Parser.h"
#include "platform/BidirectionalPipe.h"
#include "misc/EventQueue.h"
#include "platform/SymbolManager.h"
#include "SymbolParser.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class Controller {
private:
    Misc::SmartPointer<StringHandlerManager> string_manager;
    
    Misc::SmartPointer<Platform::BidirectionalPipe> gdb_pipe;
    Misc::SmartPointer<Parser> gdb_parser;
    Misc::SmartPointer<Misc::EventQueue> event_queue;
    Misc::SmartPointer<Platform::SymbolManager> symbol_manager;
    Misc::SmartPointer<SymbolParser> symbol_parser;
public:
    Controller(Misc::SmartPointer<Platform::BidirectionalPipe> gdb_pipe,
        Misc::SmartPointer<Misc::EventQueue> event_queue, Misc::SmartPointer<Platform::SymbolManager> symbol_manager);
    virtual ~Controller();
    
    void send_command(std::string command);
    
    Misc::SmartPointer<StringHandlerManager> get_stream_manager() const { return string_manager; }
    void set_string_manager(Misc::SmartPointer<StringHandlerManager> new_handler)
        { string_manager = new_handler; }
    
    void listen();
    void process(std::string line);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
