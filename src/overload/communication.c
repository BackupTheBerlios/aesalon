#include "communication.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int aesalon_overload_communication_pipefd;
int aesalon_overload_communication_enabled;

void aesalon_overload_send_string(char *string) {
    write(aesalon_overload_communication_pipefd, string, strlen(string)+1);
}
