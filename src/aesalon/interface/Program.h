#ifndef AESALON_INTERFACE_PROGRAM_H
#define AESALON_INTERFACE_PROGRAM_H

#include <string>

#include "BidirectionalPipe.h"
#include "ArgumentList.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
/* NOTE: forward declaration. */
class PipeListener;

class Program {
private:
    Misc::SmartPointer<BidirectionalPipe> gdb_pipe;
    ArgumentList argument_list;
public:
    Program();
    virtual ~Program();
    
    void execute();
};

} // namespace Interface
} // namespace Aesalon

#endif
