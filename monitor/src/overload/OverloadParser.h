#ifndef AESALON_OVERLOAD_PARSER_H
#define AESALON_OVERLOAD_PARSER_H

#include <pthread.h>
#include "Types.h"

class OverloadParser {
private:
    int pipe_fd;
    bool full_backtraces;
    pthread_t thread;
public:
    OverloadParser(int pipe_fd, bool full_backtraces);
    ~OverloadParser();
private:
    static void do_read(int fd, void *data, int wanted_size);
    static void *parse(void *pipe_fd);
};

#endif
