#ifndef AESALON_OVERLOAD_H
#define AESALON_OVERLOAD_H

#include <stddef.h>

extern void *(*aesalon_overload_malloc_ptr)(size_t);
extern void *(*aesalon_overload_realloc_ptr)(void *, size_t);
extern void (*aesalon_overload_free_ptr)(void *);
extern int aesalon_overload_communication_pipefd;
extern int aesalon_overload_communication_enabled;

#endif
