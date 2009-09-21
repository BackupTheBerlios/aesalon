#include <unistd.h>

#include "comm.h"

int pipe_fd;

void init_communication(void) {
    static int initialized = 0;
    if(initialized) return;
    pipe_fd = /* NOTE: need to get pipe_fd (write end of pipe) from somewhere */0;
}

void send_string(char *data, size_t size) {
    write(pipe_fd, data, size);
}

