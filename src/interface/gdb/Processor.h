#ifndef AESALON_INTERFACE_GDB_PROCESSOR_H
#define AESALON_INTERFACE_GDB_PROCESSOR_H

#include "Parser.h"
#include "platform/BidirectionalPipe.h"
#include "misc/EventQueue.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class Processor {
public:
    enum gdb_state_e {
        GDB_STOPPED,
        GDB_PAUSED,
        GDB_RUNNING,
        GDB_STATES
    };
private:
    gdb_state_e gdb_state;

    Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe;
    Misc::SmartPointer<Parser> parser;
    Misc::SmartPointer<Misc::EventQueue> event_queue;
    
    std::string line;
    
    void handle_async(Misc::SmartPointer<AsyncOutput> output);
    void handle_result(Misc::SmartPointer<ResultRecord> record);
public:
    Processor(Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe, Misc::SmartPointer<Misc::EventQueue> event_queue);
    virtual ~Processor() {}
    
    void process(std::string line);
    
    gdb_state_e get_gdb_state() const { return gdb_state; }
    
    bool begins_with(std::string beginning) {
        return line.substr(0, beginning.length()) == beginning;
    }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
