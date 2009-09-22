#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <stddef.h>

#include "common.h"
#include "stack_magic.h"
#include "passthrough.h"

void __attribute__((constructor)) initialize_library() {
    printf("Loading library . . .\n");
}

void *malloc(size_t size) {
    if(!initialized) return real_malloc(size);
    void *memory; 
    memory = real_malloc(size);
    
    char buffer[BUFSIZ];
    char *scope = get_previous_scope();
#if defined(_BSD_SOURCE) || _XOPEN_SOURCE >= 500 || defined(_ISOC99_SOURCE)
    snprintf(buffer, sizeof(buffer), "malloc:\"%s\":%x:%l", scope, memory, size);
#else
    sprintf(buffer, "malloc:\"%s\":%x:%l", scope, memory, size);
#endif
    real_free(scope);
    
    init_communication();
    send_string(buffer);
    
    return memory;
}
