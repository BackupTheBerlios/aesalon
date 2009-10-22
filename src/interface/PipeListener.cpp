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

void *PipeListener::create_thread(void *listener_instance) {
    PipeListener *pl = reinterpret_cast<PipeListener *>(listener_instance);
    
    pl->run();
    
    return NULL;
}

PipeListener::PipeListener(Misc::SmartPointer<BidirectionalPipe> pipe) : pipe(pipe) {
    pthread_create(&thread_id, NULL, create_thread, this);
}

PipeListener::~PipeListener() {
    pthread_cancel(thread_id);
}

void PipeListener::run() {
    while(pipe->is_open()) {
        std::string data = pipe->get_string();
        
        bool from_gdb = false;
        /* TODO: Check if the string received is from gdb or not . . . */
        if(!from_gdb) {
            output_string(data);
            continue;
        }
        
        Misc::EventQueue::lock_mutex();
        handle_gdb_string(data);
        Misc::EventQueue::unlock_mutex();
    }
}

void PipeListener::handle_gdb_string(std::string string) {
    /* TODO: Process event and add it onto the queue. Event queue mutex is already
        locked. */
}

void PipeListener::output_string(std::string string) {
    /* TODO: passthrough string, as it is not from GDB */
}

} // namespace Interface
} // namespace Aesalon
