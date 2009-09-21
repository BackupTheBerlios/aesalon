#include <dlfcn.h>
#include <string.h>

#include "stack_magic.h"
#include "passthrough.h"

char *get_previous_scope() {
    char *buffer;
    buffer = real_malloc(1024);
    
    strcpy(buffer, "<unknown scope>");
    
    return buffer;
}
