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
#include "MemoryEvent.h"

namespace Aesalon {
namespace Interface {

std::size_t PipeListener::get_size_t() {
    std::string st_str = get_string();
    std::stringstream ss;
    ss << st_str;
    std::size_t st;
    ss >> st;
    return st;
}

std::size_t PipeListener::get_address() {
    std::string add_str = get_string();
    std::stringstream ss;
    ss << add_str;
    std::size_t st;
    ss >> std::hex >> st;
    return st;
}

std::string PipeListener::get_string() {
    std::string str = get_buffer()->substr(0, get_buffer()->find(':'));
    get_buffer()->erase(0, get_buffer()->find(':')+1);
    return str;
}

void PipeListener::handle_buffer() {
    /* buffer should be one of:
        malloc:call_address:allocated_memory_address:allocated_memory_size
        realloc:call_address:allocated_memory_address:allocated_memory_size:original_memory_address
        free:call_address:freed_memory_address
        Note that all addresses are in hexidecimal form with no leading 0x.
        
        Also, be permissive of errors . . . if an error is encountered, simply return.
    */
    
    Misc::EventQueue::lock_mutex();
    
    std::string call_type = get_string();
    std::size_t call_address = get_address();
    std::size_t mem_address = get_address();
    if(call_type == "malloc") {
        std::size_t mem_size = get_size_t();
        
        MemoryEvent *me = new MallocEvent(get_program()->resolve_address(call_address), mem_address, mem_size);
        
        Misc::EventQueue::get_instance()->push_event(me);
    }
    else if(call_type == "realloc") {
        std::size_t mem_size = get_size_t();
        std::size_t mem_new_address = get_address();
        
        MemoryEvent *me = new ReallocEvent(get_program()->resolve_address(call_address), mem_address, mem_size, mem_new_address);
        
        Misc::EventQueue::get_instance()->push_event(me);
    }
    else if(call_type == "free") {
        FreeEvent *fe = new FreeEvent(get_program()->resolve_address(call_address), mem_address);
        
        Misc::EventQueue::get_instance()->push_event(fe);
    }
    
    Misc::EventQueue::unlock_mutex();
}

void PipeListener::listen() {
    char c;
    bool first = true;
    std::string temporary_buffer;
    while(read(get_pipe()->get_pipe_fd(), &c, sizeof(char))) {
        if(c != 0) temporary_buffer += c;
        else {
            if(first) {
                long thread_pid;
                sscanf(temporary_buffer.c_str(), "%ld", &thread_pid);
                listen_thread_pid = (pid_t)thread_pid;
                std::cout << "Child PID: " << listen_thread_pid << std::endl;
                first = false;
            }
            else {
                buffer = temporary_buffer;
                handle_buffer();
            }
            temporary_buffer.clear();
        }
    }
    pthread_exit(NULL);
}

PipeListener::~PipeListener() {
    pthread_cancel(listen_thread);
}

bool PipeListener::is_running() {
    int status;
    /* NOTE: Linux-specific (requires 2.6.10 or later) */
    waitpid(listen_thread_pid, &status, WNOHANG | WCONTINUED);
    return !WIFEXITED(status);
    
}

} // namespace Interface
} // namespace Aesalon
