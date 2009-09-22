#include "overload.h"
#include "communication.h"
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

void *(*aesalon_overload_malloc_ptr)(size_t);
void *(*aesalon_overload_realloc_ptr)(void *, size_t);
void (*aesalon_overload_free_ptr)(void *);


void __attribute__((constructor)) construct() {
    aesalon_overload_malloc_ptr = dlsym(RTLD_NEXT, "malloc");
    aesalon_overload_realloc_ptr = dlsym(RTLD_NEXT, "realloc");
    aesalon_overload_free_ptr = dlsym(RTLD_NEXT, "free");
    
    char *pipe_fd;
    pipe_fd = getenv("AESALON_BOOTSTRAP_PIPE");
    if(pipe_fd) sscanf(pipe_fd, "%i", &aesalon_overload_communication_pipefd);
    
    aesalon_overload_communication_enabled = 1;
}

void *malloc(size_t size) {
    char buffer[1024];
    void *memory = aesalon_overload_malloc_ptr(size);
    snprintf(buffer, 1024, "malloc:%s:%x:%i\0", "<unknown scope>", memory, size);
    aesalon_overload_send_string(buffer);
    return memory;
}

void free(void *ptr) {
    
    aesalon_overload_free_ptr(ptr);
}
