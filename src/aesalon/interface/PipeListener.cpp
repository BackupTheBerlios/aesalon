#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sstream>

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
    std::string add_str = "0x" + get_string();
    std::stringstream ss;
    ss << add_str;
    std::size_t st;
    ss >> st;
    return st;
}

std::string PipeListener::get_string() {
    std::string str = get_buffer().substr(0, get_buffer().find(':'));
    get_buffer().erase(0, get_buffer().find(':')+1);
    return str;
}

void PipeListener::handle_buffer() {
    std::string buffer = get_buffer();
    /* buffer should be one of:
        malloc:call_address:allocated_memory_address:allocated_memory_size
        realloc:call_address:allocated_memory_address:allocated_memory_size:original_memory_address
        free:call_address:freed_memory_address
        Note that all addresses are in hexidecimal form with no leading 0x.
        
        Also, be permissive of errors . . . if an error is encountered, simply return.
    */
    std::string call_type = get_string();
    std::size_t call_address = get_address();
    if(call_type == "malloc") {
        std::size_t mem_address = get_address();
        std::size_t mem_size = get_size_t();
        
        MemoryEvent *me = new MallocEvent(get_program()->resolve_address(call_address), mem_address, mem_size);
        
        Misc::EventQueue::get_instance()->push_event(me);
    }
    else if(call_type == "free") {
        std::size_t mem_address = get_address();
        
        FreeEvent *fe = new FreeEvent(get_program()->resolve_address(call_address), mem_address);
        
        Misc::EventQueue::get_instance()->push_event(fe);
    }
}

void PipeListener::listen() {
    char c;
    while(read(get_pipe()->get_pipe_fd(), &c, sizeof(char))) {
        if(c != 0) buffer += c;
        else {
            handle_buffer();
            buffer.clear();
        }
    }
}

} // namespace Interface
} // namespace Aesalon
