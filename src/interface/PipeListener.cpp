#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <sys/wait.h>

#include "PipeListener.h"
#include "misc/EventQueue.h"

namespace Aesalon {
namespace Interface {

PipeListener::PipeListener(Misc::SmartPointer<BidirectionalPipe> pipe) : pipe(pipe) {
    /* TODO: Create another thread out of the run() method */
}

PipeListener::~PipeListener() {
    
}

void PipeListener::run() {
    while(pipe->is_connected()) {
        std::string data = pipe->get_string();
        
        std::cout << "Got data: " << data << std::endl;
    }
}

} // namespace Interface
} // namespace Aesalon
