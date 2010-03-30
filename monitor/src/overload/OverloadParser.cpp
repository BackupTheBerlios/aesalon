#include <iostream>
#include <cstring>
#include <errno.h>
#include "OverloadParser.h"
#include "exception/OverloadException.h"
#include "common.h"
#include "Initializer.h"
#include "event/BlockEvent.h"

OverloadParser::OverloadParser(int pipe_fd) : pipe_fd(pipe_fd) {

}

OverloadParser::~OverloadParser() {
    close(pipe_fd);
}

void OverloadParser::parse() {
    std::cout << "OverloadParser::parse(): called . . ." << std::endl;
    unsigned char type;
    if(read(pipe_fd, &type, sizeof(type)) == -1) {
        /*throw Exception::OverloadException(Misc::StreamAsString() << "Couldn't read type from pipe: " << strerror(errno));*/
        return;
    }
    std::cout << "OverloadParser::parse(): received type: " << type << std::endl;
    
    if(type == ALLOC_TYPE) {
        allocation_data_u data;
        int bytes = read(pipe_fd, &data, sizeof(data));
        if(bytes == -1) {
            /*throw Exception::OverloadException(Misc::StreamAsString() << "Couldn't read data from pipe: " << strerror(errno));*/
            return;
        }
        else if(bytes != sizeof(type)) {
            /*throw Exception::OverloadException(Misc::StreamAsString() << "Incomplete data from pipe!");*/
            return;
        }
        Initializer::get_instance()->get_event_queue()->push_event(new Event::BlockEvent(Event::BlockEvent::ALLOC_EVENT, data.data.address, data.data.size));
    }
    else if(type == FREE_TYPE) {
        free_data_u data;
        int bytes = read(pipe_fd, &data, sizeof(data));
        if(bytes == -1) {
            /*throw Exception::OverloadException(Misc::StreamAsString() << "Couldn't read data from pipe: " << strerror(errno));*/
            return;
        }
        else if(bytes != sizeof(type)) {
            /*throw Exception::OverloadException(Misc::StreamAsString() << "Incomplete data from pipe!");*/
            return;
        }
        Initializer::get_instance()->get_event_queue()->push_event(new Event::BlockEvent(Event::BlockEvent::FREE_EVENT, data.data.address));
    }
    else if(type == REALLOC_TYPE) {
        reallocation_data_u data;
        int bytes = read(pipe_fd, &data, sizeof(data));
        if(bytes == -1) {
            /*throw Exception::OverloadException(Misc::StreamAsString() << "Couldn't read data from pipe: " << strerror(errno));*/
            return;
        }
        else if(bytes != sizeof(type)) {
            /*throw Exception::OverloadException(Misc::StreamAsString() << "Incomplete data from pipe!");*/
            return;
        }
        Initializer::get_instance()->get_event_queue()->push_event(new Event::BlockEvent(Event::BlockEvent::REALLOC_EVENT, data.data.original_address, data.data.new_size, data.data.new_address));
    }
}
