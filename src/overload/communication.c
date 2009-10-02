#include "communication.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int aesalon_overload_communication_pipefd;
int aesalon_overload_communication_enabled;

void aesalon_overload_send_string(char *string) {
    if(aesalon_overload_communication_enabled) {
        write(aesalon_overload_communication_pipefd, string, strlen(string)+1);
        /*printf("Sending string: \"%s\"\n", string);*/
    }
}
