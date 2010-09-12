#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "collector/Interface.h"

/* Prototypes copied from the GNU man page for malloc(3). */
void AC_EXPORT *calloc(size_t nmemb, size_t size);
void AC_EXPORT *malloc(size_t size);
void AC_EXPORT free(void *ptr);
void AC_EXPORT *realloc(void *ptr, size_t size);

/* Internally-used. */
static void *(*ACM_originalCalloc)(size_t memb, size_t size) = 0;
static void *(*ACM_originalMalloc)(size_t size) = 0;
static void (*ACM_originalFree)(void *ptr) = 0;
static void *(*ACM_originalRealloc)(void *ptr, size_t size) = 0;

static void ACM_initialize();
static int ACM_initialized = 0;

AC_moduleDefinition();

void AC_CONSTRUCTOR AC_constructor() {
	if(!ACM_initialized) ACM_initialize();
}

void AC_DESTRUCTOR AC_destructor() {
	
}

void ACM_initialize() {
	printf("Initializing dynamicMemory . . .\n");
	AC_Address libcOffset = AC_libraryOffset("libc-");
	
	printf("Finding calloc() offset . . .\n");
	
	ACM_originalCalloc = (void *)(libcOffset + AC_configurationInt("dynamicMemory", "callocOffset"));
	
	printf("Found calloc() offset.\n");

	ACM_initialized = 1;
	
	*(void **)(&ACM_originalMalloc) = dlsym(RTLD_NEXT, "malloc");
	*(void **)(&ACM_originalFree) = dlsym(RTLD_NEXT, "free");
	*(void **)(&ACM_originalRealloc) = dlsym(RTLD_NEXT, "realloc");

	printf("Registering module . . .\n");
	AC_registerModule("dynamicMemory");
}

void AC_EXPORT *calloc(size_t nmemb, size_t size) {
	printf("calloc(): %p . . .\n", ACM_originalCalloc);
	if(!ACM_initialized) ACM_initialize();
	printf("Calling original calloc() . . .\n");
	printf("\tcalloc: %p\n", ACM_originalCalloc);
	void *data = ACM_originalCalloc(nmemb, size);
	
	printf("Original calloc() called . . .\n");
	
	if(AC_hasCollectionBegun()) {
		/* ID byte, address, nmemb, size. */
		uint8_t toSend[1 + 8 + 8 + 8];
		toSend[0] = 0x00;
		memcpy(toSend + 1, &data, 8);
		memcpy(toSend + 9, &nmemb, 8);
		memcpy(toSend + 17, &size, 8);
		AC_DataPacket packet;
		packet.dataSource.timestamp = AC_timestamp();
		packet.dataSource.moduleID = AC_moduleID();
		packet.dataSize = sizeof(toSend);
		packet.data = toSend;
		AC_writePacket(&packet);
	}
	
	printf("Returning from calloc() . . .\n");
	return data;
}

void AC_EXPORT *malloc(size_t size) {
	printf("malloc(): %p . . .\n", ACM_originalMalloc);
	if(!ACM_initialized) ACM_initialize();
	printf("Calling originalMalloc() . . .\n");
	void *data = ACM_originalMalloc(size);
	
	if(AC_hasCollectionBegun()) {
		/* ID byte, address, size. */
		uint8_t toSend[1 + 8 + 8];
		toSend[0] = 0x01;
		memcpy(toSend + 1, &data, 8);
		memcpy(toSend + 9, &size, 8);
		AC_DataPacket packet;
		packet.dataSource.timestamp = AC_timestamp();
		packet.dataSource.moduleID = AC_moduleID();
		packet.dataSize = sizeof(toSend);
		packet.data = toSend;
		AC_writePacket(&packet);
	}
	
	return data;
}

void AC_EXPORT free(void *ptr) {
	printf("free() . . .\n");
	if(!ACM_initialized) ACM_initialize();
	if(AC_hasCollectionBegun()) {
		/* ID byte, address. */
		uint8_t toSend[1 + 8];
		toSend[0] = 0x02;
		memcpy(toSend + 1, &ptr, 8);
		AC_DataPacket packet;
		packet.dataSource.timestamp = AC_timestamp();
		packet.dataSource.moduleID = AC_moduleID();
		packet.dataSize = sizeof(toSend);
		packet.data = toSend;
		AC_writePacket(&packet);
	}
	
	ACM_originalFree(ptr);
}

void AC_EXPORT *realloc(void *ptr, size_t size) {
	printf("realloc() . . .\n");
	if(!ACM_initialized) ACM_initialize();
	
	void *data = ACM_originalRealloc(ptr, size);
	if(AC_hasCollectionBegun()) {
		/* TODO: send new address. */
		/* ID byte, address, size. */
		uint8_t toSend[1 + 8 + 8];
		toSend[0] = 0x03;
		memcpy(toSend + 1, &ptr, 8);
		memcpy(toSend + 9, &size, 8);
		AC_DataPacket packet;
		packet.dataSource.timestamp = AC_timestamp();
		packet.dataSource.moduleID = AC_moduleID();
		packet.dataSize = sizeof(toSend);
		packet.data = toSend;
		AC_writePacket(&packet);
	}
	
	return data;
}
