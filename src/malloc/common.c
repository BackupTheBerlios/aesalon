#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>

#include "common.h"

int pipe_fd;
void *libc;
int initialized;

void __attribute__((constructor)) aesalon_intialize() {
    libc = dlopen("libc.so.6", RTLD_LAZY | RTLD_DEEPBIND);
}

void __attribute__((destructor)) aesalon_deinitialize() {
    dlclose(libc);
}

void send_string(char *data) {
    write(pipe_fd, data, strlen(data)+1);
}

