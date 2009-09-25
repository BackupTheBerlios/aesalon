#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif /* #ifndef _GNU_SOURCE */
#include "overload.h"
#include "communication.h"
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>

void *(*aesalon_overload_malloc_ptr)(size_t);
void *(*aesalon_overload_realloc_ptr)(void *, size_t);
void (*aesalon_overload_free_ptr)(void *);

void *get_rp() {
#if AESALON_PLATFORM == AESALON_x86_64
    __asm__("mov rax, [rbp + 16]");
#elif AESALON_PLATFORM == AESALON_x86
    __asm__("mov eax, [rbp + 8]");
#else
    #error "No AESALON_PLATFORM defined."
#endif
}

void __attribute__((constructor)) construct() {
    char *pipe_fd;
    *(void **) (&aesalon_overload_malloc_ptr) = dlsym(RTLD_NEXT, "malloc");
    *(void **) (&aesalon_overload_realloc_ptr) = dlsym(RTLD_NEXT, "realloc");
    *(void **) (&aesalon_overload_free_ptr) = dlsym(RTLD_NEXT, "free");
    
    pipe_fd = getenv("AESALON_OVERLOAD_PIPE");
    if(pipe_fd) {
        sscanf(pipe_fd, "%i", &aesalon_overload_communication_pipefd);
        aesalon_overload_communication_enabled = 1;
    }
}

void __attribute__((destructor)) destruct() {
    if(aesalon_overload_communication_enabled)
        close(aesalon_overload_communication_pipefd);
}

void *malloc(size_t size) {
    char buffer[1024];
    void *memory = aesalon_overload_malloc_ptr(size);
    void *scope_address;
    
    get_return_pointer();
    
    snprintf(buffer, 1024, "malloc:%lx:%lx:%li", (size_t)scope_address, (size_t)memory, size);
    aesalon_overload_send_string(buffer);
    return memory;
}

void *realloc(void *ptr, size_t size) {
    char buffer[1024];
    void *memory;
    void *scope_address;
    
    get_return_pointer();
    
    memory = aesalon_overload_realloc_ptr(ptr, size);
    snprintf(buffer, 1024, "realloc:%lx:%lx:%li:%lx", (size_t)scope_address, (size_t)ptr, size, (size_t)memory);
    aesalon_overload_send_string(buffer);
    return memory;
}

void free(void *ptr) {
    char buffer[1024];
    void *scope_address;
    
    get_return_pointer();
    
    snprintf(buffer, 1024, "free:%lx:%lx", (size_t)scope_address, (size_t)ptr);
    aesalon_overload_send_string(buffer);
    aesalon_overload_free_ptr(ptr);
}
