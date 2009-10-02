#ifndef AESALON_INTERFACE_PIPE_LISTENER_H
#define AESALON_INTERFACE_PIPE_LISTENER_H

#include <iostream>
#include <pthread.h>
#include <sys/types.h>

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
    pid_t listen_thread_pid;
    
    std::string *get_buffer() const { return const_cast<std::string *>(&buffer); }
    
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
        pthread_create(&listen_thread, NULL, start_thread, this);
    }
    virtual ~PipeListener();
    
    void listen();
    
    Misc::SmartPointer<Program> get_program() const { return program; }
    Misc::SmartPointer<Pipe> get_pipe() const { return pipe; }
    
    bool is_running();
};

} // namespace Interface
} // namespace Aesalon

#endif
