#include <stdlib.h>
#include <unistd.h>

#include "PipeListener.h"

namespace Aesalon {
namespace Interface {

void PipeListener::handle_buffer() {
    
}

void PipeListener::listen() {
    char c;
    while(read(get_pipe()->get_pipe_fd(), &c, sizeof(char))) {
        if(c != 0) buffer += c;
        else handle_buffer();
    }
}

} // namespace Interface
} // namespace Aesalon
