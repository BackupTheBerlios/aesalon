#include "overload.h"

void *(*aesalon_bootstrap_malloc_ptr)(size_t);
void *(*aesalon_bootstrap_realloc_ptr)(void *, size_t);
void (*aesalon_bootstrap_free_ptr)(void *);

void *malloc(size_t size) {
    printf("Using overloaded malloc(): ptr is %x\n", aesalon_bootstrap_malloc_ptr);
    return aesalon_bootstrap_malloc_ptr(size);
}

void free(void *ptr) {
    puts("Using overloaded free() . . .");
    aesalon_bootstrap_free_ptr(ptr);
}
