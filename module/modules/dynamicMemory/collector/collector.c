#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "collector/Interface.h"

/* Copied from the GNU man page for malloc(3). */
void AC_EXPORT *calloc(size_t nmemb, size_t size);
void AC_EXPORT *malloc(size_t size);
void AC_EXPORT free(void *ptr);
void AC_EXPORT *realloc(void *ptr, size_t size);

/* Internally-used. */
static void *(*ACM_originalCalloc)(size_t memb, size_t size);
static void *(*ACM_originalMalloc)(size_t size);
static void (*ACM_originalFree)(void *ptr);
static void *(*ACM_originalRealloc)(void *ptr, size_t size);

void AC_CONSTRUCTOR AC_constructor() {
	printf("Constructing dynamicMemory module . . .\n");
	AC_Address libcOffset = AC_libraryOffset("libc-");
	ACM_originalMalloc = (void *)(libcOffset + 0x76900);
	/*void *libcHandle = dlopen("libc-2.11.2.so", RTLD_LAZY | RTLD_LOCAL | RTLD_NOLOAD);
	printf("libcHandle: %p\n", libcHandle);*/

	*(void **)(&ACM_originalCalloc) = dlsym(RTLD_NEXT, "calloc");/*dlsym(libcHandle, "calloc");*/
	*(void **)(&ACM_originalFree) = dlsym(RTLD_NEXT, "free");
	*(void **)(&ACM_originalRealloc) = dlsym(RTLD_NEXT, "realloc");
	printf("ACM_originalCalloc: %p\n", ACM_originalCalloc);
}

void AC_DESTRUCTOR AC_destructor() {
	printf("Destructing dynamicMemory module . . .\n");
}

void AC_EXPORT *calloc(size_t nmemb, size_t size) {
	printf("In calloc() . . .\n");
	printf("\tnmemb: %i\n\tsize: %i\n", nmemb, size);
	if(ACM_originalCalloc == NULL) {
		void *data = malloc(nmemb * size);
		memset(data, 0, nmemb * size);
		return data;
	}
	return NULL;
}

void AC_EXPORT *malloc(size_t size) {
	printf("In malloc() . . .\n");
	
	return ACM_originalMalloc(size);
}

void AC_EXPORT free(void *ptr) {
	printf("In free() . . .\n");
	ACM_originalFree(ptr);
}

void AC_EXPORT *realloc(void *ptr, size_t size) {
	printf("In realloc() . . .\n");
	return ACM_originalRealloc(ptr, size);
}
