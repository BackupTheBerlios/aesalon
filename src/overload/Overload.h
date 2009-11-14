#ifndef AESALON_OVERLOAD_H
#define AESALON_OVERLOAD_H

#include <stddef.h>

void __attribute__((constructor)) aesalon_overload_initialize();
void __attribute__((destructor)) aesalon_overload_deinitialize();

void *calloc(size_t nmemb, size_t size);
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);

#endif
