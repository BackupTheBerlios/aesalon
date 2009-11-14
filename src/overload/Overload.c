#define _GNU_SOURCE
#include <dlfcn.h>

#include "Overload.h"

void *(*aesalon_overload_calloc)(size_t, size_t);
void *(*aesalon_overload_malloc)(size_t);
void *(*aesalon_overload_free)();
void *(*aesalon_overload_realloc)(void *, size_t);

void __attribute__((constructor)) aesalon_overload_initialize() {
    *(void **)(&aesalon_overload_calloc) = dlsym(RTLD_NEXT, "calloc");
    *(void **)(&aesalon_overload_malloc) = dlsym(RTLD_NEXT, "malloc");
    *(void **)(&aesalon_overload_free) = dlsym(RTLD_NEXT, "free");
    *(void **)(&aesalon_overload_realloc) = dlsym(RTLD_NEXT, "realloc");
}
void __attribute__((destructor)) aesalon_overload_deinitialize() {
    
}

void *calloc(size_t nmemb, size_t size) {
    void *value = aesalon_overload_calloc(nmemb, size);
    return value;
}
void *malloc(size_t size) {
    void *value = aesalon_overload_malloc(size);
    return value;
}
void free(void *ptr) {
    aesalon_overload_free(ptr);
}
void *realloc(void *ptr, size_t size) {
    void *value = aesalon_overload_realloc(ptr, size);
    return value;
}
