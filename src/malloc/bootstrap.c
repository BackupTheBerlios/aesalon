#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>

#include "bootstrap.h"

void *aesalon_bootstrap_libc;
void *aesalon_bootstrap_overload;
void *(*aesalon_bootstrap_malloc_ptr)(size_t);
void *(*aesalon_bootstrap_realloc_ptr)(void *, size_t);
void *(*aesalon_bootstrap_free_ptr)(void *);
int aesalon_bootstrap_use_overload = 0;

void __attribute__((constructor)) aesalon_bootstrap_construct() {
    puts("Constructing bootstrap shared library");
    aesalon_bootstrap_libc = dlopen("libc6.so.6", RTLD_LAZY);
    
    aesalon_bootstrap_malloc_ptr = dlsym(aesalon_bootstrap_libc, "malloc");
    aesalon_bootstrap_realloc_ptr = dlsym(aesalon_bootstrap_libc, "realloc");
    aesalon_bootstrap_free_ptr = dlsym(aesalon_bootstrap_libc, "free");
    
    /* Load overload shared library */
    aesalon_bootstrap_overload = dlopen("libaesalon_overload.so", RTLD_LAZY | RTLD_GLOBAL | RTLD_DEEPBIND);
    
    /* Set global pointers */
    void *(**malloc_ptr)(size_t);
    malloc_ptr = dlsym(aesalon_bootstrap_overload, "aesalon_bootstrap_malloc_ptr");
    *malloc_ptr = aesalon_bootstrap_malloc_ptr;
    void *(**free_ptr)(void *);
    free_ptr = dlsym(aesalon_bootstrap_overload, "aesalon_bootstrap_free_ptr");
    *free_ptr = aesalon_bootstrap_free_ptr;
    
    aesalon_bootstrap_use_overload = 1;
    
    printf("Original malloc(): %x \tNew malloc: %x\n", aesalon_bootstrap_malloc_ptr, dlsym(NULL, "malloc"));
}

void __attribute__((destructor)) aesalon_bootstrap_destruct() {
    puts("Destructing bootstrap shared library");
    aesalon_bootstrap_use_overload = 0;
}

void *aesalon_bootstrap_malloc(size_t size) {
    return aesalon_bootstrap_malloc_ptr(size);
}

void *aesalon_bootstrap_realloc(void *ptr, size_t size) {
    return aesalon_bootstrap_realloc_ptr(ptr, size);
}

void aesalon_bootstrap_free(void *ptr) {
    aesalon_bootstrap_free_ptr(ptr);
}
