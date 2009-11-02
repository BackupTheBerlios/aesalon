#ifndef AESALON_INTERFACE_GDB_PROCESSOR_H
#define AESALON_INTERFACE_GDB_PROCESSOR_H

#include "Parser.h"
#include "platform/BidirectionalPipe.h"
#include "misc/EventQueue.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class Processor {
private:
    Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe;
    Misc::SmartPointer<Parser> parser;
    
    std::string line;
public:
    Processor(Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe);
    virtual ~Processor() {}
    
    void process(Misc::EventQueue &queue);
    
    bool begins_with(std::string beginning) {
        return line.substr(0, beginning.length()) == beginning;
    }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
