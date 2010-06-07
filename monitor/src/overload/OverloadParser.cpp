#include <iostream>
#include <cstring>
#include <errno.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "OverloadParser.h"
#include "exception/OverloadException.h"
#include "common.h"
#include "Initializer.h"
#include "event/BlockEvent.h"
#include "event/MonitorEvent.h"
#include "misc/Message.h"
#include "misc/String.h"

OverloadParser::OverloadParser(pid_t pid) : pid(pid), full_backtraces(false) {
    std::string shm_filename = Misc::StreamAsString() << "/aesalon-overload-shm-" << pid;
    int shm_fd = shm_open(shm_filename.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    
    Misc::String::to<Word>(Initializer::get_instance()->get_argument_parser()->get_argument("shm-size")->get_data(), shared_memory_size);
    ftruncate(shm_fd, shared_memory_size);
    
    shared_memory = static_cast<Byte *>(mmap(NULL, shared_memory_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    
    shared_memory_header = reinterpret_cast<shm_header_t *>(shared_memory);
    
    sem_init(&shared_memory_header->begin_sem, 1, 0);
    shared_memory_header->begin = 0;
    sem_init(&shared_memory_header->end_sem, 1, 1);
    shared_memory_header->end = 0;
    shared_memory_header->data_offset = sizeof(*shared_memory_header) + 1;
    
    full_backtraces = !Initializer::get_instance()->get_argument_parser()->get_argument("no-backtrace")->is_found();
    
    pthread_create(&thread, NULL, parse, (void *)this);
}

OverloadParser::~OverloadParser() {
    Misc::Message(Misc::Message::DEBUG_MESSAGE, "Sending backlog . . .");
    pthread_join(thread, NULL);
    
    munmap(shared_memory, shared_memory_size);
    
    std::string shm_filename = Misc::StreamAsString() << "/aesalon-overload-shm-" << pid;
    shm_unlink(shm_filename.c_str());
}

void OverloadParser::do_read(void *data, int wanted_size) {
    
}

void *OverloadParser::parse(void *overload_parser) {
    OverloadParser *parser = reinterpret_cast<OverloadParser *>(overload_parser);
    
    unsigned char type;
    while(true) {
        parser->do_read(&type, sizeof(type));
        Event::BlockEvent *event = NULL;
        if(type == ALLOC_TYPE) {
            allocation_data_u data;
            parser->do_read(&data, sizeof(data));
            event = new Event::BlockEvent(Event::BlockEvent::ALLOC_EVENT, data.data.timestamp, data.data.address, data.data.size);
        }
        else if(type == FREE_TYPE) {
            free_data_u data;
            parser->do_read(&data, sizeof(data));
            event = new Event::BlockEvent(Event::BlockEvent::FREE_EVENT, data.data.timestamp, data.data.address);
        }
        else if(type == REALLOC_TYPE) {
            reallocation_data_u data;
            parser->do_read(&data, sizeof(data));
            event = new Event::BlockEvent(Event::BlockEvent::REALLOC_EVENT, data.data.timestamp, data.data.original_address, data.data.new_size, data.data.new_address);
        }
        if(event) {
            u_int32_t scope_size;
            parser->do_read(&scope_size, sizeof(scope_size));
            if(scope_size > 0) {
                Word *scopes = (Word *)malloc(sizeof(Word) * scope_size);
                parser->do_read(scopes, sizeof(Word) * scope_size);
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
