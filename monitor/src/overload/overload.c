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

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>

void __attribute__((constructor)) aesalon_constructor();
void __attribute__((destructor)) aesalon_destructor();

void *get_scope_address();

void *calloc(size_t nmemb, size_t size);
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);

int pipe_fd;

void __attribute__((constructor)) aesalon_constructor() {
    char *pipe_str = getenv("aesalon_pipe_fd");
    if(pipe_str == NULL) {
        fprintf(stderr, "{aesalon} Failed to initialize overload: aesalon_pipe_fd environment variable not set.\n");
        exit(1);
    }
    sscanf(pipe_str, "%i", &pipe_fd);
}

void __attribute__((destructor)) aesalon_destructor() {
    if(pipe_fd) close(pipe_fd);
}

void* get_scope_address() {
    asm("mov rax, [rbp + 16]");
}

void *calloc(size_t nmemb, size_t size) {
    void *data;
    void *(*original_calloc)(size_t nmemb, size_t size);
    *(void **) (&original_calloc) = dlsym(RTLD_NEXT, "calloc");
    
    asm("push [rbp + 8]");
    data = get_scope_address();
    asm("add rsp, 8");
    
    char buffer[25];
    buffer[0] = 0x00;
    buffer[1] = (unsigned long)data & 0xff;
    buffer[2] = ((unsigned long)data << 8) & 0xff;
    buffer[3] = ((unsigned long)data << 16) & 0xff;
    buffer[4] = ((unsigned long)data << 24) & 0xff;
    buffer[5] = ((unsigned long)data << 32) & 0xff;
    buffer[6] = ((unsigned long)data << 40) & 0xff;
    buffer[7] = ((unsigned long)data << 48) & 0xff;
    buffer[8] = ((unsigned long)data << 56) & 0xff;
    
    data = (void *)(nmemb * size);
    
    buffer[17] = (unsigned long)data & 0xff;
    buffer[18] = ((unsigned long)data << 8) & 0xff;
    buffer[19] = ((unsigned long)data << 16) & 0xff;
    buffer[20] = ((unsigned long)data << 24) & 0xff;
    buffer[21] = ((unsigned long)data << 32) & 0xff;
    buffer[22] = ((unsigned long)data << 40) & 0xff;
    buffer[23] = ((unsigned long)data << 48) & 0xff;
    buffer[24] = ((unsigned long)data << 56) & 0xff;
    
    data = original_calloc(nmemb, size);
    
    buffer[9] = (unsigned long)data & 0xff;
    buffer[10] = ((unsigned long)data << 8) & 0xff;
    buffer[11] = ((unsigned long)data << 16) & 0xff;
    buffer[12] = ((unsigned long)data << 24) & 0xff;
    buffer[13] = ((unsigned long)data << 32) & 0xff;
    buffer[14] = ((unsigned long)data << 40) & 0xff;
    buffer[15] = ((unsigned long)data << 48) & 0xff;
    buffer[16] = ((unsigned long)data << 56) & 0xff;
    
    write(pipe_fd, buffer, sizeof(buffer));
    
    return data;
}

void *malloc(size_t size) {
    void *data;
    void *(*original_malloc)(size_t size);
    *(void **) (&original_malloc) = dlsym(RTLD_NEXT, "malloc");
    
    asm("push [rbp + 8]");
    data = get_scope_address();
    asm("add rsp, 8");
    
    char buffer[25];
    buffer[0] = 0x00;
    buffer[1] = (unsigned long)data & 0xff;
    buffer[2] = ((unsigned long)data << 8) & 0xff;
    buffer[3] = ((unsigned long)data << 16) & 0xff;
    buffer[4] = ((unsigned long)data << 24) & 0xff;
    buffer[5] = ((unsigned long)data << 32) & 0xff;
    buffer[6] = ((unsigned long)data << 40) & 0xff;
    buffer[7] = ((unsigned long)data << 48) & 0xff;
    buffer[8] = ((unsigned long)data << 56) & 0xff;
    
    data = (void *)size;
    
    buffer[17] = (unsigned long)data & 0xff;
    buffer[18] = ((unsigned long)data << 8) & 0xff;
    buffer[19] = ((unsigned long)data << 16) & 0xff;
    buffer[20] = ((unsigned long)data << 24) & 0xff;
    buffer[21] = ((unsigned long)data << 32) & 0xff;
    buffer[22] = ((unsigned long)data << 40) & 0xff;
    buffer[23] = ((unsigned long)data << 48) & 0xff;
    buffer[24] = ((unsigned long)data << 56) & 0xff;
    
    data = original_malloc(size);
    
    buffer[9] = (unsigned long)data & 0xff;
    buffer[10] = ((unsigned long)data << 8) & 0xff;
    buffer[11] = ((unsigned long)data << 16) & 0xff;
    buffer[12] = ((unsigned long)data << 24) & 0xff;
    buffer[13] = ((unsigned long)data << 32) & 0xff;
    buffer[14] = ((unsigned long)data << 40) & 0xff;
    buffer[15] = ((unsigned long)data << 48) & 0xff;
    buffer[16] = ((unsigned long)data << 56) & 0xff;
    
    write(pipe_fd, buffer, sizeof(buffer));
    
    return data;
}

void free(void *ptr) {
    void *data;
    void *(*original_free)(void *ptr);
    *(void **) (&original_free) = dlsym(RTLD_NEXT, "free");
    
    asm("push [rbp + 8]");
    data = get_scope_address();
    asm("add rsp, 8");
    
    char buffer[17];
    buffer[0] = 0x02;
    buffer[1] = (unsigned long)data & 0xff;
    buffer[2] = ((unsigned long)data << 8) & 0xff;
    buffer[3] = ((unsigned long)data << 16) & 0xff;
    buffer[4] = ((unsigned long)data << 24) & 0xff;
    buffer[5] = ((unsigned long)data << 32) & 0xff;
    buffer[6] = ((unsigned long)data << 40) & 0xff;
    buffer[7] = ((unsigned long)data << 48) & 0xff;
    buffer[8] = ((unsigned long)data << 56) & 0xff;
    
    data = ptr;
    
    buffer[9] = (unsigned long)data & 0xff;
    buffer[10] = ((unsigned long)data << 8) & 0xff;
    buffer[11] = ((unsigned long)data << 16) & 0xff;
    buffer[12] = ((unsigned long)data << 24) & 0xff;
    buffer[13] = ((unsigned long)data << 32) & 0xff;
    buffer[14] = ((unsigned long)data << 40) & 0xff;
    buffer[15] = ((unsigned long)data << 48) & 0xff;
    buffer[16] = ((unsigned long)data << 56) & 0xff;
    
    original_free(ptr);
    
    write(pipe_fd, buffer, sizeof(buffer));
}

void *realloc(void *ptr, size_t size) {
    void *data;
    void *(*original_realloc)(void *ptr, size_t size);
    *(void **) (&original_realloc) = dlsym(RTLD_NEXT, "realloc");
    
    asm("push [rbp + 8]");
    data = get_scope_address();
    asm("add rsp, 8");
    
    char buffer[33];
    buffer[0] = 0x01;
    buffer[1] = (unsigned long)data & 0xff;
    buffer[2] = ((unsigned long)data << 8) & 0xff;
    buffer[3] = ((unsigned long)data << 16) & 0xff;
    buffer[4] = ((unsigned long)data << 24) & 0xff;
    buffer[5] = ((unsigned long)data << 32) & 0xff;
    buffer[6] = ((unsigned long)data << 40) & 0xff;
    buffer[7] = ((unsigned long)data << 48) & 0xff;
    buffer[8] = ((unsigned long)data << 56) & 0xff;
    
    data = ptr;
    
    buffer[9] = (unsigned long)data & 0xff;
    buffer[10] = ((unsigned long)data << 8) & 0xff;
    buffer[11] = ((unsigned long)data << 16) & 0xff;
    buffer[12] = ((unsigned long)data << 24) & 0xff;
    buffer[13] = ((unsigned long)data << 32) & 0xff;
    buffer[14] = ((unsigned long)data << 40) & 0xff;
    buffer[15] = ((unsigned long)data << 48) & 0xff;
    buffer[16] = ((unsigned long)data << 56) & 0xff;
    
    data = (void *)size;
    
    buffer[17] = (unsigned long)data & 0xff;
    buffer[18] = ((unsigned long)data << 8) & 0xff;
    buffer[19] = ((unsigned long)data << 16) & 0xff;
    buffer[20] = ((unsigned long)data << 24) & 0xff;
    buffer[21] = ((unsigned long)data << 32) & 0xff;
    buffer[22] = ((unsigned long)data << 40) & 0xff;
    buffer[23] = ((unsigned long)data << 48) & 0xff;
    buffer[24] = ((unsigned long)data << 56) & 0xff;
    
    data = original_realloc(ptr, size);

    buffer[25] = (unsigned long)data & 0xff;
    buffer[26] = ((unsigned long)data << 8) & 0xff;
    buffer[27] = ((unsigned long)data << 16) & 0xff;
    buffer[28] = ((unsigned long)data << 24) & 0xff;
    buffer[29] = ((unsigned long)data << 32) & 0xff;
    buffer[30] = ((unsigned long)data << 40) & 0xff;
    buffer[31] = ((unsigned long)data << 48) & 0xff;
    buffer[32] = ((unsigned long)data << 56) & 0xff;
    
    write(pipe_fd, buffer, sizeof(buffer));
    
    return data;
}
