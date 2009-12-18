#ifndef AESALON_INTERFACE_PROGRAM_MANAGER_H
#define AESALON_INTERFACE_PROGRAM_MANAGER_H

#include "misc/SmartPointer.h"
#include "platform/BidirectionalPipe.h"
#include "platform/ArgumentList.h"

namespace Aesalon {
namespace Interface {

class ProgramManager {
private:
    Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe;
    Misc::SmartPointer<Platform::ArgumentList> argument_list;
public:
    ProgramManager(Misc::SmartPointer<Platform::ArgumentList> argument_list)
        : bi_pipe(NULL), argument_list(argument_list) {}
    virtual ~ProgramManager() {}
    
    void execute();
    
    Misc::SmartPointer<Platform::ArgumentList> get_argument_list() const { return argument_list; }
};

} // namespace Interface
} // namespace Aesalon

#endif
