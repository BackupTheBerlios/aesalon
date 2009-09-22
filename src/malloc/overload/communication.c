#include "communication.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int aesalon_communication_pipefd;

void aesalon_overload_send_string(char *string) {
    /*char *pipe_fd;
    pipe_fd = getenv("AESALON_BOOTSTRAP_PIPE");
    if(pipe_fd) sscanf(pipe_fd, "%i", &aesalon_bootstrap_communication_pipefd);*/
    
    /*write(aesalon_bootstrap_communication_pipefd, string, strlen(string)+1);*/
}
