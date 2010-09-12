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

AC_moduleDefinition;

void AC_CONSTRUCTOR AC_constructor() {
	AC_Address libcOffset = AC_libraryOffset("libc-");
	
	ACM_originalCalloc = (void *)(libcOffset + AC_configurationInt("dynamicMemory", "callocOffset"));
	
	*(void **)(&ACM_originalMalloc) = dlsym(RTLD_NEXT, "malloc");
	*(void **)(&ACM_originalFree) = dlsym(RTLD_NEXT, "free");
	*(void **)(&ACM_originalRealloc) = dlsym(RTLD_NEXT, "realloc");
	
	AC_registerModule("dynamicMemory");
}

void AC_DESTRUCTOR AC_destructor() {
	
}

void AC_EXPORT *calloc(size_t nmemb, size_t size) {
	void *data = ACM_originalCalloc(nmemb, size);
	
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
	
	return data;
}

void AC_EXPORT *malloc(size_t size) {
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
	if(AC_hasCollectionBegun()) {
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
	
	return ACM_originalRealloc(ptr, size);
}
