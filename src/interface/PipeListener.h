#ifndef AESALON_INTERFACE_PIPE_LISTENER_H
#define AESALON_INTERFACE_PIPE_LISTENER_H

#include <iostream>
#include <pthread.h>
#include <sys/types.h>

#include "BidirectionalPipe.h"
#include "Program.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {

/** UNIX pipe listener class; interprets data coming down the pipe from gdb. */
class PipeListener {
private:
    Misc::SmartPointer<BidirectionalPipe> pipe;
    pthread_t thread_id;
    pid_t thread_pid;
    
    void handle_gdb_string(std::string string);
    static void *create_thread(void *listener_instance);
    
    void run();
    
    void output_string(std::string string);
public:
    PipeListener(Misc::SmartPointer<BidirectionalPipe> pipe);
    virtual ~PipeListener();
};

} // namespace Interface
} // namespace Aesalon

#endif
