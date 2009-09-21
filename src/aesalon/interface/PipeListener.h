#ifndef AESALON_INTERFACE_PIPE_LISTENER_H
#define AESALON_INTERFACE_PIPE_LISTENER_H

#include "Pipe.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {

class PipeListener {
private:
    Misc::SmartPointer<Pipe> pipe;
    unsigned int sleep_time;
    std::string buffer;
    
    std::string get_buffer() const { return buffer; }
    
    void handle_buffer();
public:
    PipeListener(Misc::SmartPointer<Pipe> pipe, unsigned int sleep_time = 30) :
        pipe(pipe), sleep_time(sleep_time) {}
    
    void listen();
    
    Misc::SmartPointer<Pipe> get_pipe() const { return pipe; }
};

} // namespace Interface
} // namespace Aesalon

#endif
