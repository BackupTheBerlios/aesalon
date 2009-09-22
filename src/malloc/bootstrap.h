#ifndef AESALON_BOOTSTRAP_H
#define AESALON_BOOTSTRAP_H

void __attribute__((constructor)) aesalon_bootstrap_construct();
void __attribute__((destructor)) aesalon_bootstrap_destruct();

void *aesalon_bootstrap_malloc(size_t size);
void *aesalon_bootstrap_realloc(void *ptr, size_t size);
void aesalon_bootstrap_free(void *ptr);

extern void *aesalon_bootstrap_libc;
extern void *aesalon_bootstrap_overload;
extern void *(*aesalon_bootstrap_malloc_ptr)(size_t);
extern void *(*aesalon_bootstrap_realloc_ptr)(void *, size_t);
extern void *(*aesalon_bootstrap_free_ptr)(void *);
extern int aesalon_bootstrap_use_overload;

#endif
