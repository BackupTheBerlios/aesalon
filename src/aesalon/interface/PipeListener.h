#ifndef AESALON_INTERFACE_PIPE_LISTENER_H
#define AESALON_INTERFACE_PIPE_LISTENER_H

#include <iostream>
#include <pthread.h>

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
    pthread_t listen_thread;
    
    std::string get_buffer() const { return buffer; }
    
    void handle_buffer();
    
    std::size_t get_size_t();
    std::size_t get_address();
    std::string get_string();
    
    static void *start_thread(void *object) {
        /* NOTE: this reinterpret_cast is dangerous . . . */
        reinterpret_cast<PipeListener *>(object)->listen();
        return NULL;
    }
public:
    PipeListener(Misc::SmartPointer<Pipe> pipe, Misc::SmartPointer<Program> program) :
        pipe(pipe), program(program) {
        std::cout << "Creating PipeListener . . ." << std::endl;
        pthread_create(&listen_thread, NULL, start_thread, this);
    }
    
    void listen();
    
    Misc::SmartPointer<Program> get_program() const { return program; }
    Misc::SmartPointer<Pipe> get_pipe() const { return pipe; }
};

} // namespace Interface
} // namespace Aesalon

#endif
