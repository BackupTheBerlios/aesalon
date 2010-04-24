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

/* Prototypes copied from respective man pages. */

void *calloc(size_t nmemb, size_t size);
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);

int posix_memalign(void **memptr, size_t alignment, size_t size);
void *valloc(size_t size);
void *memalign(size_t boundary, size_t size);

void *(*original_calloc)(size_t nmemb, size_t size);
void *(*original_malloc)(size_t size);
void *(*original_free)(void *ptr);
void *(*original_realloc)(void *ptr, size_t size);

int (*original_posix_memalign)(void **memptr, size_t alignment, size_t size);
void *(*original_valloc)(size_t size);
void *(*original_memalign)(size_t boundary, size_t size);

unsigned long get_libc_offset(char *libc_path);

int pipe_fd;

void initialize_overload();
void deinitialize_overload();
void write_bt_info();

int overload_initialized = 0;

void __attribute__((constructor)) aesalon_constructor() {
    /* NOTE: the constructor may be called after other libs if there is more than one preload. */
    if(!overload_initialized) initialize_overload();
}

void __attribute__((destructor)) aesalon_destructor() {
    deinitialize_overload();
}

void *calloc(size_t nmemb, size_t size) {
    if(!overload_initialized) initialize_overload();
    allocation_data_u data;    
    static unsigned char type = ALLOC_TYPE;
    
    data.data.size = nmemb * size;
    if(original_calloc != NULL) data.data.address = (unsigned long)original_calloc(nmemb, size);
    else {
        data.data.address = (unsigned long)original_malloc(data.data.size);
        memset((void *)data.data.address, 0, data.data.size);
    }
    
    write(pipe_fd, &type, sizeof(type));
    write(pipe_fd, data.buffer, sizeof(data.buffer));
    
    write_bt_info();
    
    return (void *)data.data.address;
}

void *malloc(size_t size) {
    if(!overload_initialized) initialize_overload();
    allocation_data_u data;
    static unsigned char type = ALLOC_TYPE;
    
    data.data.address = (unsigned long)original_malloc(size);
    data.data.size = size;
    
    write(pipe_fd, &type, sizeof(type));
    write(pipe_fd, data.buffer, sizeof(data.buffer));
    
    write_bt_info();
    
    return (void *)data.data.address;
}

void free(void *ptr) {
    if(!overload_initialized) initialize_overload();
    free_data_u data;
    static unsigned char type = FREE_TYPE;
    
    data.data.address = (unsigned long)ptr;
    original_free(ptr);
    
    int ret = 0;
    ret = write(pipe_fd, &type, sizeof(type));
    ret = write(pipe_fd, data.buffer, sizeof(data.buffer));
    
    write_bt_info();
}

void *realloc(void *ptr, size_t size) {
    if(!overload_initialized) initialize_overload();
    reallocation_data_u data;
    static unsigned char type = REALLOC_TYPE;
    
    data.data.original_address = (unsigned long)ptr;
    
    data.data.new_size = size;
    
    data.data.new_address = (unsigned long)original_realloc(ptr, size);
    
    write(pipe_fd, &type, sizeof(type));
    write(pipe_fd, data.buffer, sizeof(data.buffer));
    
    write_bt_info();
    
    return (void *)data.data.new_address;
}

int posix_memalign(void** memptr, size_t alignment, size_t size) {
    if(!overload_initialized) initialize_overload();
    allocation_data_u data;
    static unsigned char type = ALLOC_TYPE;
    int result = original_posix_memalign(memptr, alignment, size);
    
    data.data.address = (unsigned long)*memptr;
    data.data.size = size;
    
    write(pipe_fd, &type, sizeof(type));
    write(pipe_fd, data.buffer, sizeof(data.buffer));
    
    write_bt_info();
    
    return result;
}

void *valloc(size_t size) {
    if(!overload_initialized) initialize_overload();
    allocation_data_u data;
    static unsigned char type = ALLOC_TYPE;
    
    data.data.address = (unsigned long)original_valloc(size);
    data.data.size = size;
    
    write(pipe_fd, &type, sizeof(type));
    write(pipe_fd, data.buffer, sizeof(data.buffer));
    
    write_bt_info();
    
    return (void *)data.data.address;
}

void *memalign(size_t boundary, size_t size) {
    if(!overload_initialized) initialize_overload();
    allocation_data_u data;
    static unsigned char type = ALLOC_TYPE;
    
    data.data.address = (unsigned long)original_memalign(boundary, size);
    data.data.size = size;
    
    write(pipe_fd, &type, sizeof(type));
    write(pipe_fd, data.buffer, sizeof(data.buffer));
    
    write_bt_info();
    
    return (void *)data.data.address;
}

long unsigned int get_libc_offset(char *libc_path) {
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

        if(strcmp(str, libc_path) == 0) {
            break;
        }
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
    char *libc_path = getenv("aesalon_libc_path");
    if(libc_path == NULL) {
        fprintf(stderr, "{aesalon} Failed to initialize overload: aesalon_libc_path environment variable not set.\n");
        exit(1);
    }
    unsigned long libc_offset = get_libc_offset(libc_path);
    original_malloc += libc_offset;
#ifdef DEVELOPMENT_BUILD
    printf("{aesalon} Resolving symbols . . .\n");
#endif
    *(void **) (&original_calloc) = dlsym(RTLD_NEXT, "calloc");
    *(void **) (&original_free) = dlsym(RTLD_NEXT, "free");
    *(void **) (&original_realloc) = dlsym(RTLD_NEXT, "realloc");
    *(void **) (&original_memalign) = dlsym(RTLD_NEXT, "posix_memalign");
    *(void **) (&original_valloc) = dlsym(RTLD_NEXT, "valloc");
    *(void **) (&original_memalign) = dlsym(RTLD_NEXT, "memalign");
#ifdef DEVELOPMENT_BUILD
    printf("{aesalon} Resolved symbols.\n");
    printf("{aesalon} Overload library initialization completed.\n");
#endif
}

void deinitialize_overload() {
#ifdef DEVELOPMENT_BUILD
    printf("{aesalon} Overload library self-destructing . . .\n");
#endif
    if(pipe_fd) close(pipe_fd);    
}

void write_bt_info() {
    void *bp = NULL;
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    asm("mov qword [rbp-0x28], rbp");
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
    asm("mov qword [ebp-0x0c], ebp");
#endif
    
    u_int32_t buffer_alloc = 1;
    u_int32_t buffer_size = 0;

    unsigned char *buffer = NULL;

    int address_size = sizeof(unsigned long);
    
    unsigned long bt_address = 0x0;
    
    bp = (void *)*((unsigned long *)bp);
    
    do {
        printf("bp: %p\n", bp);
        bt_address = *((unsigned long *)(bp + 8));
        if((buffer_size + address_size) > buffer_alloc) {
            while((buffer_size + address_size) > buffer_alloc) buffer_alloc *= 2;
            buffer = original_realloc(buffer, buffer_alloc);
            memcpy(buffer + buffer_size, &bt_address, address_size);
            buffer_size += address_size;
        }
        printf("backtrace address: %p\n", bt_address);
        bp = (void *)*((unsigned long *)bp);
    } while(bp != NULL && bt_address != 0);
    
    buffer_size /= 4;
    
    write(pipe_fd, &buffer_size, sizeof(u_int32_t));
    write(pipe_fd, buffer, (buffer_size - 1) * sizeof(u_int32_t));
    
    original_free(buffer);
}

#ifdef __cplusplus
}
#endif
