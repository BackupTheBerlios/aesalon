#ifndef AESALON_INTERFACE_GDB_CONTROLLER_H
#define AESALON_INTERFACE_GDB_CONTROLLER_H

#include <string>

#include "StreamHandler.h"
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
    Misc::SmartPointer<StreamHandler> stream_handler;
    
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
    
    Misc::SmartPointer<StreamHandler> get_stream_handler() const { return stream_handler; }
    void set_stream_handler(Misc::SmartPointer<StreamHandler> new_handler)
        { stream_handler = new_handler; }
    
    void listen();
    void process(std::string line);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
