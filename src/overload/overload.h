#ifndef AESALON_OVERLOAD_H
#define AESALON_OVERLOAD_H

#include <stddef.h>

#define AESALON_x86 0
#define AESALON_x86_64 1

#if AESALON_PLATFORM == AESALON_x86_64
#define get_return_pointer() \
    __asm__("push [rbp+8]"); \
    scope_address = get_rp(); \
    __asm__("add rsp, 8");
#elif AESALON_PLATFORM == AESALON_x86
#define get_return_pointer() \
    __asm__("push [rbp+4]"); \
    scope_address = get_rp(); \
    __asm__("add rsp, 4");
#else
    #error "No AESALON_PLATFORM defined."
#endif

extern void *(*aesalon_overload_malloc_ptr)(size_t);
extern void *(*aesalon_overload_realloc_ptr)(void *, size_t);
extern void (*aesalon_overload_free_ptr)(void *);
extern int aesalon_overload_communication_pipefd;
extern int aesalon_overload_communication_enabled;

#endif
