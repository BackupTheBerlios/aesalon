#include <stdlib.h>
#include <unistd.h>
#include <sstream>

#include "PipeListener.h"
#include "misc/EventQueue.h"
#include "MemoryEvent.h"

namespace Aesalon {
namespace Interface {

void PipeListener::handle_buffer() {
    std::string buffer = get_buffer();
    /* NOTE: ADD ERROR CHECKING */
    std::string type = buffer.substr(0, buffer.find(':')-1);
    buffer.erase(0, buffer.find(':'));
    std::string scope = buffer.substr(0, buffer.find(':')-1);
    buffer.erase(0, buffer.find(':'));
    if(type == "malloc") {
        std::string address_str = buffer.substr(0, buffer.find(':')-1);
        buffer.erase(0, buffer.find(':'));
        std::string size_str = buffer;
        
        std::size_t address, size;
        
        std::stringstream ss;
        
        ss << address_str;
        ss >> address;
        ss.clear();
        
        ss << size_str;
        ss >> size;
        ss.clear();
        
        MemoryEvent *me;
        me = new MallocEvent(scope, address, size);
        
        Misc::EventQueue::get_instance()->push_event(me);
    }
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
