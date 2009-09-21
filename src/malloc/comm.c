#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "comm.h"

int pipe_fd;

void init_communication(void) {
    static int initialized = 0;
    if(initialized) return;
    
    char *fd_string = getenv("AESALON_PIPE");
    /* if fd_string is NULL, then this is not being run within the aesalon interface. Ignore it. */
    if(!fd_string) {
        return;
    }
    sscanf(fd_string, "%i", &pipe_fd);
    initialized = 1;
}

void send_string(char *data) {
    write(pipe_fd, data, strlen(data)+1);
}

