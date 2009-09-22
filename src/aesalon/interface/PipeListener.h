#ifndef AESALON_INTERFACE_PIPE_LISTENER_H
#define AESALON_INTERFACE_PIPE_LISTENER_H

#include "Pipe.h"
#include "Program.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {

class PipeListener {
private:
    Misc::SmartPointer<Pipe> pipe;
    Misc::SmartPointer<Program> program;
    std::string buffer;
    
    std::string get_buffer() const { return buffer; }
    
    void handle_buffer();
    
    std::size_t get_size_t();
    std::size_t get_address();
    std::string get_string();
public:
    PipeListener(Misc::SmartPointer<Pipe> pipe, Misc::SmartPointer<Program> program) :
        pipe(pipe), program(program) {}
    
    void listen();
    
    Misc::SmartPointer<Program> get_program() const { return program; }
    Misc::SmartPointer<Pipe> get_pipe() const { return pipe; }
};

} // namespace Interface
} // namespace Aesalon

#endif
