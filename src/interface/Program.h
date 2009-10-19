#ifndef AESALON_INTERFACE_PROGRAM_H
#define AESALON_INTERFACE_PROGRAM_H

#include <string>

#include "BidirectionalPipe.h"
#include "PipeListener.h"
#include "ArgumentList.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
/* NOTE: forward declaration. */
class PipeListener;

class Program {
private:
    Misc::SmartPointer<BidirectionalPipe> gdb_pipe;
    Misc::SmartPointer<PipeListener> pipe_listener;
    ArgumentList argument_list;
public:
    Program();
    virtual ~Program();
    
    void execute(std::string executable);
};

} // namespace Interface
} // namespace Aesalon

#endif
