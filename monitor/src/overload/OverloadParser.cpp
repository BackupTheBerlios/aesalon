#include <iostream>
#include <cstring>
#include <errno.h>
#include "OverloadParser.h"
#include "exception/OverloadException.h"
#include "common.h"
#include "Initializer.h"
#include "event/BlockEvent.h"
#include "misc/Message.h"

OverloadParser::OverloadParser(int pipe_fd) : pipe_fd(pipe_fd) {
    pthread_create(&thread, NULL, parse, (void *)pipe_fd);
}

OverloadParser::~OverloadParser() {
    Misc::Message(Misc::Message::DEBUG_MESSAGE, "Sending backlog . . .");
    pthread_join(thread, NULL);
    close(pipe_fd);
}

void *OverloadParser::parse(void *fd) {
    int pipe_fd = (long)fd;
    unsigned char type;
    int ret = 0;
    while((ret = read(pipe_fd, &type, sizeof(type))) == 1) {
        Event::BasicEvent *event = NULL;
        if(type == ALLOC_TYPE) {
            allocation_data_u data;
            int bytes = read(pipe_fd, &data, sizeof(data));
            if(bytes == -1) {
                /*throw Exception::OverloadException(Misc::StreamAsString() << "Couldn't read data from pipe: " << strerror(errno));*/
                return NULL;
            }
            else if(bytes != sizeof(data)) {
                /*throw Exception::OverloadException(Misc::StreamAsString() << "Incomplete data from pipe!");*/
                continue;
            }
            event = new Event::BlockEvent(Event::BlockEvent::ALLOC_EVENT, 0, data.data.address, data.data.size);
        }
        else if(type == FREE_TYPE) {
            free_data_u data;
            int bytes = read(pipe_fd, &data, sizeof(data));
            if(bytes == -1) {
                /*throw Exception::OverloadException(Misc::StreamAsString() << "Couldn't read data from pipe: " << strerror(errno));*/
                return NULL;
            }
            else if(bytes != sizeof(data)) {
                /*throw Exception::OverloadException(Misc::StreamAsString() << "Incomplete data from pipe!");*/
                continue;
            }
            event = new Event::BlockEvent(Event::BlockEvent::FREE_EVENT, 0, data.data.address);
        }
        else if(type == REALLOC_TYPE) {
            reallocation_data_u data;
            int bytes = read(pipe_fd, &data, sizeof(data));
            if(bytes == -1) {
                /*throw Exception::OverloadException(Misc::StreamAsString() << "Couldn't read data from pipe: " << strerror(errno));*/
                return NULL;
            }
            else if(bytes != sizeof(data)) {
                /*throw Exception::OverloadException(Misc::StreamAsString() << "Incomplete data from pipe!");*/
                continue;
            }
            event = new Event::BlockEvent(Event::BlockEvent::REALLOC_EVENT, 0, data.data.original_address, data.data.new_size, data.data.new_address);
        }
        if(event) {
            Initializer::get_instance()->get_event_queue()->lock_mutex();
            Initializer::get_instance()->get_event_queue()->push_event(event);
            Initializer::get_instance()->get_event_queue()->unlock_mutex();
        }
    }
    return NULL;
}
