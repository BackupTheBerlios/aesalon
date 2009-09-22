#ifndef AESALON_OVERLOAD_H
#define AESALON_OVERLOAD_H

#include <stddef.h>

extern void *(*aesalon_bootstrap_malloc_ptr)(size_t);
extern void *(*aesalon_bootstrap_realloc_ptr)(void *, size_t);
extern void (*aesalon_bootstrap_free_ptr)(void *);
extern int aesalon_bootstrap_communication_pipefd;

#endif
