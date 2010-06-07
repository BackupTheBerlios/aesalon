#ifndef AESALON_OVERLOAD_PARSER_H
#define AESALON_OVERLOAD_PARSER_H

#include <sys/types.h>
#include <pthread.h>

#include "Types.h"
#include "common.h"

class OverloadParser {
private:
    pid_t pid;
    bool full_backtraces;
    pthread_t thread;
    Byte *shared_memory;
    Word shared_memory_size;
    shm_header_t *shared_memory_header;
public:
    OverloadParser(pid_t pid);
    ~OverloadParser();
    
    void do_read(void *data, int wanted_size);
private:
    static void *parse(void *overload_parser);
};

#endif
