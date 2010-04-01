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
    unsigned char type;
    if(read(pipe_fd, &type, sizeof(type)) != sizeof(type)) {
        /*throw Exception::OverloadException(Misc::StreamAsString() << "Couldn't read type from pipe: " << strerror(errno));*/
        return;
    }
    std::cout << "type: " << int(type) << std::endl;
    if(type == ALLOC_TYPE) {
        allocation_data_u data;
        int bytes = read(pipe_fd, &data, sizeof(data));
        if(bytes == -1) {
            /*throw Exception::OverloadException(Misc::StreamAsString() << "Couldn't read data from pipe: " << strerror(errno));*/
            return;
        }
        else if(bytes != sizeof(data)) {
            /*throw Exception::OverloadException(Misc::StreamAsString() << "Incomplete data from pipe!");*/
            return;
        }
        Initializer::get_instance()->get_event_queue()->push_event(new Event::BlockEvent(Event::BlockEvent::ALLOC_EVENT, data.data.scope, data.data.address, data.data.size));
    }
    else if(type == FREE_TYPE) {
        std::cout << "found FREE_TYPE . . ." << std::endl;
        free_data_u data;
        int bytes = read(pipe_fd, &data, sizeof(data));
        if(bytes == -1) {
            /*throw Exception::OverloadException(Misc::StreamAsString() << "Couldn't read data from pipe: " << strerror(errno));*/
            return;
        }
        else if(bytes != sizeof(data)) {
            /*throw Exception::OverloadException(Misc::StreamAsString() << "Incomplete data from pipe!");*/
            return;
        }
        std::cout << "OverloadParser: pushing FreeEvent onto queue . . ." << std::endl;
        Initializer::get_instance()->get_event_queue()->push_event(new Event::BlockEvent(Event::BlockEvent::FREE_EVENT, data.data.scope, data.data.address));
    }
    else if(type == REALLOC_TYPE) {
        reallocation_data_u data;
        int bytes = read(pipe_fd, &data, sizeof(data));
        if(bytes == -1) {
            /*throw Exception::OverloadException(Misc::StreamAsString() << "Couldn't read data from pipe: " << strerror(errno));*/
            return;
        }
        else if(bytes != sizeof(data)) {
            /*throw Exception::OverloadException(Misc::StreamAsString() << "Incomplete data from pipe!");*/
            return;
        }
        Initializer::get_instance()->get_event_queue()->push_event(new Event::BlockEvent(Event::BlockEvent::REALLOC_EVENT, data.data.scope, data.data.original_address, data.data.new_size, data.data.new_address));
    }
}
