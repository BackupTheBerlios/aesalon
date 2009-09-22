#include <dlfcn.h>
#include <stddef.h>
#include <stdlib.h>

#include "passthrough.h"

extern void *libc;

void *real_malloc(size_t size) {
    void *(*r_malloc)(size_t);
    r_malloc = dlsym(libc, "malloc");
    void *memory;
    if(r_malloc) memory = r_malloc(size);
    else return NULL;
    dlclose(libc);
    return memory;
}

void real_free(void *memory) {
    void *(*r_free)(void *);
    r_free = dlsym(libc, "free");
    if(r_free) r_free(memory);
    else return;
    dlclose(libc);
}
