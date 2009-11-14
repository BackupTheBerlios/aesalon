#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Overload.h"

void __attribute__((visibility ("hidden"))) *(*aesalon_overload_calloc)(size_t, size_t);
void __attribute__((visibility ("hidden"))) *(*aesalon_overload_malloc)(size_t);
void __attribute__((visibility ("hidden"))) *(*aesalon_overload_free)();
void __attribute__((visibility ("hidden"))) *(*aesalon_overload_realloc)(void *, size_t);
FILE __attribute__((visibility ("hidden"))) *aesalon_overload_pipe;
int __attribute__((visibility ("hidden"))) aesalon_overload_initialized;

void __attribute__((constructor)) aesalon_overload_initialize() {
    *(void **)(&aesalon_overload_calloc) = dlsym(RTLD_NEXT, "calloc");
    *(void **)(&aesalon_overload_malloc) = dlsym(RTLD_NEXT, "malloc");
    *(void **)(&aesalon_overload_free) = dlsym(RTLD_NEXT, "free");
    *(void **)(&aesalon_overload_realloc) = dlsym(RTLD_NEXT, "realloc");
    
    char named_pipe[1024];
    
    strcpy(named_pipe, "/tmp/aesalon_overload-");
    
    strcat(named_pipe, getenv("AESALON_PID"));
    
    aesalon_overload_pipe = fopen(named_pipe, "wt");
    aesalon_overload_initialized = 1;
}

void __attribute__((destructor)) aesalon_overload_deinitialize() {
    fclose(aesalon_overload_pipe);
}

void *calloc(size_t nmemb, size_t size) {
    void *value = aesalon_overload_calloc(nmemb, size);
    if(aesalon_overload_initialized) fprintf(aesalon_overload_pipe, "calloc:%li:%li:%p\n", nmemb, size, value);
    return value;
}
void *malloc(size_t size) {
    void *value = aesalon_overload_malloc(size);
    if(aesalon_overload_initialized) fprintf(aesalon_overload_pipe, "malloc:%li:%p\n", size, value);
    return value;
}
void free(void *ptr) {
    aesalon_overload_free(ptr);
    if(aesalon_overload_initialized) fprintf(aesalon_overload_pipe, "free:%p\n", ptr);
}
void *realloc(void *ptr, size_t size) {
    void *value = aesalon_overload_realloc(ptr, size);
    if(aesalon_overload_initialized) fprintf(aesalon_overload_pipe, "realloc:%p:%li:%p\n", ptr, size, value);
    return value;
}
