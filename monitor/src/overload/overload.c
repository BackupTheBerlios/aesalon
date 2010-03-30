/** Aesalon, a project to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009-2010 strange <kawk256@gmail.com>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    @file overload/overload.c
*/

#include "common.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

void __attribute__((constructor)) aesalon_constructor();
void __attribute__((destructor)) aesalon_destructor();

void *get_scope_address();

/*void *calloc(size_t nmemb, size_t size);*/
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);

void *(*original_calloc)(size_t nmemb, size_t size);
void *(*original_malloc)(size_t size);
void *(*original_free)(void *ptr);
void *(*original_realloc)(void *ptr, size_t size);

int pipe_fd;

void __attribute__((constructor)) aesalon_constructor() {
#ifdef DEVELOPMENT_BUILD
    printf("{aesalon} Initializing overload library . . .\n");
#endif
    char *pipe_str = getenv("aesalon_pipe_fd");
    if(pipe_str == NULL) {
        fprintf(stderr, "{aesalon} Failed to initialize overload: aesalon_pipe_fd environment variable not set.\n");
        exit(1);
    }
    sscanf(pipe_str, "%i", &pipe_fd);
#ifdef DEVELOPMENT_BUILD
    printf("{aesalon} found pipe fd (%i)\n", pipe_fd);
    
    printf("{aesalon} Resolving symbols . . .\n");
    printf("{aesalon} Resolving calloc . . .\n");
#endif
    *(void **) (&original_calloc) = dlsym(RTLD_NEXT, "calloc");
#ifdef DEVELOPMENT_BUILD
    printf("{aesalon} Resolved calloc (%p).\n", original_calloc);
    printf("{aesalon} Resolving malloc . . .\n");
#endif
    *(void **) (&original_malloc) = dlsym(RTLD_NEXT, "malloc");
#ifdef DEVELOPMENT_BUILD
    printf("{aesalon} Resolved malloc (%p).\n", original_malloc);
    printf("{aesalon} Resolving free . . .\n");
#endif
    *(void **) (&original_free) = dlsym(RTLD_NEXT, "free");
#ifdef DEVELOPMENT_BUILD
    printf("{aesalon} Resolved free (%p).\n", original_free);
    printf("{aesalon} Resolving realloc . . .\n");
#endif
    *(void **) (&original_realloc) = dlsym(RTLD_NEXT, "realloc");
#ifdef DEVELOPMENT_BUILD
    printf("{aesalon} Resolved realloc (%p).\n", original_realloc);
    printf("{aesalon} Overload library initialization completed.\n");
#endif
}

void __attribute__((destructor)) aesalon_destructor() {
#ifdef DEVELOPMENT_BUILD
    printf("{aesalon} Overload library self-destructing . . .\n");
#endif
    if(pipe_fd) close(pipe_fd);
}

void* get_scope_address() {
    asm("mov rax, [rbp + 16]");
}

/*void *calloc(size_t nmemb, size_t size) {
    allocation_data_u data;    
    static unsigned char type = ALLOC_TYPE;
    
    asm("push [rbp + 8]");
    data.data.scope = (unsigned long)get_scope_address();
    asm("add rsp, 8");
    
    data.data.address = (unsigned long)original_calloc(nmemb, size);
    data.data.size = nmemb * size;
    
    write(pipe_fd, &type, sizeof(type));
    write(pipe_fd, data.buffer, sizeof(data.buffer));
    
    return (void *)data.data.address;
}*/

void *malloc(size_t size) {
    allocation_data_u data;
    static unsigned char type = ALLOC_TYPE;
    
    asm("push [rbp + 8]");
    data.data.scope = (unsigned long)get_scope_address();
    asm("add rsp, 8");
    
    data.data.address = (unsigned long)original_malloc(size);
    data.data.size = size;
    
    write(pipe_fd, &type, sizeof(type));
    write(pipe_fd, data.buffer, sizeof(data.buffer));
    
    return (void *)data.data.address;
}

void free(void *ptr) {
    free_data_u data;
    static unsigned char type = FREE_TYPE;
    
    asm("push [rbp + 8]");
    data.data.scope = (unsigned long)get_scope_address();
    asm("add rsp, 8");
    
    data.data.address = (unsigned long)ptr;
    original_free(ptr);
    
    write(pipe_fd, &type, sizeof(type));
    write(pipe_fd, data.buffer, sizeof(data.buffer));
}

void *realloc(void *ptr, size_t size) {
    reallocation_data_u data;
    static unsigned char type = REALLOC_TYPE;
    
    asm("push [rbp + 8]");
    data.data.scope = (unsigned long)get_scope_address();
    asm("add rsp, 8");
    
    data.data.original_address = (unsigned long)ptr;
    
    data.data.new_size = size;
    
    data.data.new_address = (unsigned long)original_realloc(ptr, size);
    
    write(pipe_fd, &type, sizeof(type));
    write(pipe_fd, data.buffer, sizeof(data.buffer));
    
    return (void *)data.data.new_address;
}

#ifdef __cplusplus
}
#endif
