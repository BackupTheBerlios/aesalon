#include <iostream>
#include <cstring>
#include <errno.h>
#include <stdlib.h>
#include "OverloadParser.h"
#include "exception/OverloadException.h"
#include "common.h"
#include "Initializer.h"
#include "event/BlockEvent.h"
#include "event/MonitorEvent.h"
#include "misc/Message.h"

OverloadParser::OverloadParser(int pipe_fd, bool full_backtraces) : pipe_fd(pipe_fd), full_backtraces(full_backtraces) {
    pthread_create(&thread, NULL, parse, (void *)pipe_fd);
}

OverloadParser::~OverloadParser() {
    Misc::Message(Misc::Message::DEBUG_MESSAGE, "Sending backlog . . .");
    pthread_join(thread, NULL);
    close(pipe_fd);
}

void OverloadParser::do_read(int fd, void *data, int wanted_size) {
    int read_bytes = 0;
    int bytes = 0;
    while(read_bytes < wanted_size && (bytes = read(fd, ((unsigned char *)data) + read_bytes, wanted_size - read_bytes)) != -1) {
        /* If bytes is zero, then read() returned zero, which means that an EOF was detected.
            (read() is a blocking function) */
        if(bytes == 0) {
            /* Hence, stop the thread. */
            
            Event::BasicEvent *event = new Event::MonitorEvent(Event::MonitorEvent::PROGRAM_FINISHED);
            
            Initializer::get_instance()->get_event_queue()->lock_mutex();
            Initializer::get_instance()->get_event_queue()->push_event(event);
            Initializer::get_instance()->get_event_queue()->unlock_mutex();
            
            pthread_exit(0);
            
        }
        /* If bytes is -1, then an error has occured. Check what it is . . . */
        else if(bytes == -1) {
            /* If it was interrupted by a signal, try it again . . .*/
            if(errno == EINTR) continue;
            /* Else, for all other errors, just exit. */
            pthread_exit((void *)1);
        }
        read_bytes += bytes;
    }
}

void *OverloadParser::parse(void *fd) {
    int pipe_fd = (long)fd;
    unsigned char type;
    while(true) {
        do_read(pipe_fd, &type, sizeof(type));
        Event::BlockEvent *event = NULL;
        if(type == ALLOC_TYPE) {
            allocation_data_u data;
            do_read(pipe_fd, &data, sizeof(data));
            event = new Event::BlockEvent(Event::BlockEvent::ALLOC_EVENT, data.data.timestamp, data.data.address, data.data.size);
        }
        else if(type == FREE_TYPE) {
            free_data_u data;
            do_read(pipe_fd, &data, sizeof(data));
            event = new Event::BlockEvent(Event::BlockEvent::FREE_EVENT, data.data.timestamp, data.data.address);
        }
        else if(type == REALLOC_TYPE) {
            reallocation_data_u data;
            do_read(pipe_fd, &data, sizeof(data));
            event = new Event::BlockEvent(Event::BlockEvent::REALLOC_EVENT, data.data.timestamp, data.data.original_address, data.data.new_size, data.data.new_address);
        }
        if(event) {
            u_int32_t scope_size;
            do_read(pipe_fd, &scope_size, sizeof(scope_size));
            if(scope_size > 0) {
                Word *scopes = (Word *)malloc(sizeof(Word) * scope_size);
                do_read(pipe_fd, scopes, sizeof(Word) * scope_size);
                event->set_scope(scopes);
            }
            event->set_scope_size(scope_size);
            
            Initializer::get_instance()->get_event_queue()->lock_mutex();
            Initializer::get_instance()->get_event_queue()->push_event(event);
            Initializer::get_instance()->get_event_queue()->unlock_mutex();
        }
    }
    return NULL;
}
