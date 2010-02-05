#include <stdio.h>
#include <malloc.h>

void aesalon_initialize_overload();
void *aesalon_malloc_hook(size_t size, const void *caller);
void *aesalon_realloc_hook(void *ptr, size_t size, const void *caller);
void aesalon_free_hook(void *ptr, const void *caller);

void *(*aesalon_old_malloc_hook)(size_t size, const void *caller);
void *(*aesalon_old_realloc_hook)(void *ptr, size_t size, const void *caller);
void (*aesalon_old_free_hook)(void *ptr, const void *caller);
void (*__malloc_initialize_hook)(void) = aesalon_initialize_overload;


void aesalon_initialize_overload() {
    aesalon_old_malloc_hook = __malloc_hook;
    __malloc_hook = aesalon_malloc_hook;
    aesalon_old_realloc_hook = __realloc_hook;
    __realloc_hook = aesalon_realloc_hook;
    aesalon_old_free_hook = __free_hook;
    __free_hook = aesalon_free_hook;
}

void *aesalon_malloc_hook(size_t size, const void *caller) {
    void *data;
    __malloc_hook = aesalon_old_malloc_hook;
    data = malloc(size);
    __malloc_hook = aesalon_malloc_hook;
    
    /* Send off a SIGTRAP, this data is ready for collection . . . */
    asm("int3");
    
    return data;
}

void *aesalon_realloc_hook(void *ptr, size_t size, const void *caller) {
    void *data;
    __realloc_hook = aesalon_old_realloc_hook;
    data = realloc(ptr, size);
    __realloc_hook = aesalon_realloc_hook;
    
    /* Send off a SIGTRAP, this data is ready for collection . . . */
    asm("int3");
    
    return data;
}

void aesalon_free_hook(void *ptr, const void *caller) {
    __free_hook = aesalon_old_free_hook;
    free(ptr);
    __free_hook = aesalon_free_hook;
    
    asm("int3");
}
