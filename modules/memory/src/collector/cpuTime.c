#define _GNU_SOURCE

#include <stdio.h>
#include <errno.h>
#include <dlfcn.h>
#include <malloc.h>

#include "informer/Informer.h"

void AC_EXPORT *calloc(size_t nmemb, size_t size);
void AC_EXPORT *malloc(size_t size);
void AC_EXPORT free(void *ptr);
void AC_EXPORT *realloc(void *ptr, size_t size);

ModuleID AC_moduleID;
void *(*AC_callocFP)(size_t nmemb, size_t size);
void *(*AC_mallocFP)(size_t size);
void (*AC_freeFP)(void *ptr);

void AC_EXPORT *calloc(size_t nmemb, size_t size) {
	static int resolving = 0;
	static char resolvingData[1024];
	static size_t usedData = 0;
	printf("Intercepted calloc call, nmemb %li with size %li.\n", nmemb, size);
	if(resolving == 1) {
		printf("\tIn \"resolving\" mode, using local static buffer . . .\n");
		size_t original = usedData;
		usedData += nmemb * size;
		return resolvingData + original;
	}
	if(AC_callocFP == NULL) {
		resolving = 1;
		*(void **)&AC_callocFP = dlsym(RTLD_NEXT, "calloc");
		resolving = 0;
	}
	return AC_callocFP(nmemb, size);
}

void AC_EXPORT *malloc(size_t size) {
	printf("Intercepted malloc call for size %li.\n", size);
	if(AC_mallocFP == NULL) {
		*(void **)&AC_mallocFP = dlsym(RTLD_NEXT, "malloc");
	}
	return AC_mallocFP(size);
}

void AC_EXPORT free(void *ptr) {
	printf("Intercepted free call.\n");
	if(AC_freeFP == NULL) {
		*(void **)&AC_freeFP = dlsym(RTLD_NEXT, "free");
	}
	AC_freeFP(ptr);
}

void __attribute__((constructor)) AC_EXPORT AM_Construct() {
	printf("Constructing memory module . . .\n");
	AI_Construct();
	
	
	
	
	AC_moduleID = AI_ConfigurationLong("memory:moduleID");
	
	AI_ModuleLoaded("memory", AC_moduleID);
}

void __attribute__((destructor)) AC_EXPORT AM_Destruct() {
	
}
