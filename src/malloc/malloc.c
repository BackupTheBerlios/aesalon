#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <stddef.h>

#include "comm.h"
#include "stack_magic.h"
#include "passthrough.h"

void *malloc(size_t size) {
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
    send_string(buffer, strlen(buffer));
    
    return memory;
}
