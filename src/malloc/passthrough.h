#ifndef AESALON_MALLOC_PASSTHROUGH_H
#define AESALON_MALLOC_PASSTHROUGH_H

#include <stddef.h>

void *real_malloc(size_t size);
void real_free(void *memory);

#endif
