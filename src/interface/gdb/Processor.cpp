#include <string>
#include <sstream>

#include "Processor.h"
#include "platform/MemoryEvent.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

Processor::Processor(Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe) : bi_pipe(bi_pipe) {
    parser = new Parser();
}

void Processor::process(Misc::EventQueue &queue) {
    line = bi_pipe->get_string();
    
    if(!line.length()) return;
    if(begins_with("(gdb)")) return;
    
    String *string = parser->parse_gdb_string(line);
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
