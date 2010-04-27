#include <iostream>
#include <cstring>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "OverloadParser.h"
#include "exception/OverloadException.h"
#include "common.h"
#include "Initializer.h"
#include "event/BlockEvent.h"
#include "misc/Message.h"

OverloadParser::OverloadParser(pid_t pid, bool full_backtraces) : full_backtraces(full_backtraces) {
    pthread_create(&thread, NULL, parse, (void *)pid);
}

OverloadParser::~OverloadParser() {
    Misc::Message(Misc::Message::DEBUG_MESSAGE, "Sending backlog . . .");
    pthread_join(thread, NULL);
    close(pipe_fd);
}

void *OverloadParser::parse(void *pid) {
    int pipe_fd = open((Misc::StreamAsString() << TEMP_PATH << "/aesalon-" << (unsigned long)pid).operator std::string().c_str(), O_RDONLY);
    unsigned char type;
    int ret = 0;
    while((ret = read(pipe_fd, &type, sizeof(type))) == 1) {
        Event::BlockEvent *event = NULL;
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
            event = new Event::BlockEvent(Event::BlockEvent::ALLOC_EVENT, data.data.timestamp, data.data.address, data.data.size);
            uint32_t scope_count;
            read(pipe_fd, &scope_count, sizeof(scope_count));
            Word *scopes = (Word *)malloc(sizeof(Word) * scope_count);
            read(pipe_fd, scopes, sizeof(Word) * scope_count);
            event->set_scope(scopes);
            event->set_scope_size(scope_count);
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
            event = new Event::BlockEvent(Event::BlockEvent::FREE_EVENT, data.data.timestamp, data.data.address);
            uint32_t scope_count;
            read(pipe_fd, &scope_count, sizeof(scope_count));
            Word *scopes = (Word *)malloc(sizeof(Word) * scope_count);
            read(pipe_fd, scopes, sizeof(Word) * scope_count);
            event->set_scope(scopes);
            event->set_scope_size(scope_count);
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
            event = new Event::BlockEvent(Event::BlockEvent::REALLOC_EVENT, data.data.timestamp, data.data.original_address, data.data.new_size, data.data.new_address);
            uint32_t scope_count;
            read(pipe_fd, &scope_count, sizeof(scope_count));
            Word *scopes = (Word *)malloc(sizeof(Word) * scope_count);
            read(pipe_fd, scopes, sizeof(Word) * scope_count);
            event->set_scope(scopes);
            event->set_scope_size(scope_count);
        }
        if(event) {
            Initializer::get_instance()->get_event_queue()->lock_mutex();
            Initializer::get_instance()->get_event_queue()->push_event(event);
            Initializer::get_instance()->get_event_queue()->unlock_mutex();
        }
    }
    return NULL;
}
