#ifndef AESALON_OVERLOAD_PARSER_H
#define AESALON_OVERLOAD_PARSER_H

#include <pthread.h>

class OverloadParser {
private:
    int pipe_fd;
    pthread_t thread;
    
    static void *parse(void *pipe_fd);
public:
    OverloadParser(int pipe_fd);
    ~OverloadParser();
};

#endif