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
#include <fcntl.h>

#ifdef __cplusplus
extern "C" {
#endif

void __attribute__((constructor)) aesalon_constructor();
void __attribute__((destructor)) aesalon_destructor();

void *get_scope_address();

void *calloc(size_t nmemb, size_t size);
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);

void *(*original_calloc)(size_t nmemb, size_t size);
void *(*original_malloc)(size_t size);
void *(*original_free)(void *ptr);
void *(*original_realloc)(void *ptr, size_t size);

unsigned long get_libc_offset();

int pipe_fd;

void initialize_overload();
void deinitialize_overload();

int overload_initialized = 0;

void __attribute__((constructor)) aesalon_constructor() {
    /* NOTE: the constructor may be called after other libs if there is more than one preload. */
    if(!overload_initialized) initialize_overload();
}

void __attribute__((destructor)) aesalon_destructor() {
    deinitialize_overload();
}

void* get_scope_address() {
    asm("mov rax, [rbp + 16]");
}

void *calloc(size_t nmemb, size_t size) {
    if(!overload_initialized) initialize_overload();
    allocation_data_u data;    
    static unsigned char type = ALLOC_TYPE;
    
    asm("push [rbp + 8]");
    data.data.scope = (unsigned long)get_scope_address();
    asm("add rsp, 8");
    data.data.size = nmemb * size;
    if(original_calloc != NULL) data.data.address = (unsigned long)original_calloc(nmemb, size);
    else {
        data.data.address = (unsigned long)original_malloc(data.data.size);
        memset((void *)data.data.address, 0, data.data.size);
    }
    
    write(pipe_fd, &type, sizeof(type));
    write(pipe_fd, data.buffer, sizeof(data.buffer));
    
    return (void *)data.data.address;
}

void *malloc(size_t size) {
    if(!overload_initialized) initialize_overload();
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
    if(!overload_initialized) initialize_overload();
    free_data_u data;
    static unsigned char type = FREE_TYPE;
    
    asm("push [rbp + 8]");
    data.data.scope = (unsigned long)get_scope_address();
    asm("add rsp, 8");
    
    data.data.address = (unsigned long)ptr;
    original_free(ptr);
    
    int ret = 0;
    ret = write(pipe_fd, &type, sizeof(type));
    ret = write(pipe_fd, data.buffer, sizeof(data.buffer));
}

void *realloc(void *ptr, size_t size) {
    if(!overload_initialized) initialize_overload();
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

long unsigned int get_libc_offset() {
    char buffer[1024];
    
    sprintf(buffer, "/proc/%i/maps", getpid());
    
    int fd = open(buffer, O_RDONLY);
    unsigned long address = 0;
    
    int ret = 1;
    while(ret > 0) {
        char c = 0;
        int pos = 0;
        while(c != '\n' && (ret = read(fd, &c, sizeof(c)))) buffer[pos++] = c;
        buffer[pos] = 0;
        
        char str[128];
        sscanf(buffer, "%lx-%*lx %*s %*s %*s %*s %s", &address, str, str, str, str, str, str);

        if(strstr(str, "libc-")) break;
    }
    
    close(fd);
    
    return address;
}

void initialize_overload() {
    if(overload_initialized) return;
    overload_initialized = 1;
#ifdef DEVELOPMENT_BUILD
    printf("{aesalon} Initializing overload library . . .\n");
#endif
    char *pipe_str = getenv("aesalon_pipe_fd");
    if(pipe_str == NULL) {
        fprintf(stderr, "{aesalon} Failed to initialize overload: aesalon_pipe_fd environment variable not set.\n");
        exit(1);
    }
    sscanf(pipe_str, "%i", &pipe_fd);
    unsigned long malloc_offset;
    char *malloc_offset_str = getenv("aesalon_malloc_offset");
    if(pipe_str == NULL) {
        fprintf(stderr, "{aesalon} Failed to initialize overload: aesalon_malloc_offset environment variable not set.\n");
        exit(1);
    }
    sscanf(malloc_offset_str, "%lx", &original_malloc);
    unsigned long libc_offset = get_libc_offset();
    original_malloc += libc_offset;
#ifdef DEVELOPMENT_BUILD
    printf("{aesalon} found pipe fd (%i)\n", pipe_fd);
    
    printf("{aesalon} Resolving symbols . . .\n");
/*    printf("{aesalon} Resolving malloc . . .\n");
#endif
    *(void **) (&original_malloc) = dlsym(RTLD_NEXT, "malloc");
#ifdef DEVELOPMENT_BUILD
    printf("{aesalon} Resolved malloc (%p) (guess was %p (%p + %p)).\n", original_malloc, malloc_offset + libc_offset, libc_offset, malloc_offset);*/
    printf("{aesalon} Resolving calloc . . .\n");
#endif
    *(void **) (&original_calloc) = dlsym(RTLD_NEXT, "calloc");
#ifdef DEVELOPMENT_BUILD
    printf("{aesalon} Resolved calloc (%p).\n", original_calloc);
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

void deinitialize_overload() {
#ifdef DEVELOPMENT_BUILD
    printf("{aesalon} Overload library self-destructing . . .\n");
#endif
    if(pipe_fd) close(pipe_fd);    
}

#ifdef __cplusplus
}
#endif
