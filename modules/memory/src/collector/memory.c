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
void *(*AC_reallocFP)(void *ptr, size_t size);

void AC_EXPORT *calloc(size_t nmemb, size_t size) {
	static int resolving = 0;
	static char resolvingData[1024];
	static size_t usedData = 0;
	
	if(resolving == 1) {
		size_t original = usedData;
		usedData += nmemb * size;
		return resolvingData + original;
	}
	
	if(AC_callocFP == NULL) {
		resolving = 1;
		*(void **)&AC_callocFP = dlsym(RTLD_NEXT, "calloc");
		resolving = 0;
	}
	
	void *data = AC_callocFP(nmemb, size);
	
	if(AI_CollectionStatus()) {
		AI_StartPacket(AC_moduleID);
		
		*(uint8_t *)AI_PacketSpace(1) = 1;
		*(uint64_t *)AI_PacketSpace(8) = AI_Timestamp();
		*(uint64_t *)AI_PacketSpace(8) = (uint64_t)data;
		*(uint64_t *)AI_PacketSpace(8) = size;
		
		AI_EndPacket();
	}
	
	return data;
}

void AC_EXPORT *malloc(size_t size) {
	if(AC_mallocFP == NULL) {
		*(void **)&AC_mallocFP = dlsym(RTLD_NEXT, "malloc");
	}
	
	void *data = AC_mallocFP(size);
	
	if(AI_CollectionStatus()) {
		AI_StartPacket(AC_moduleID);
		
		*(uint8_t *)AI_PacketSpace(1) = 2;
		*(uint64_t *)AI_PacketSpace(8) = AI_Timestamp();
		*(uint64_t *)AI_PacketSpace(8) = (uint64_t)data;
		*(uint64_t *)AI_PacketSpace(8) = size;
		
		AI_EndPacket();
	}
	
	return data;
}

void AC_EXPORT free(void *ptr) {
	if(AC_freeFP == NULL) {
		*(void **)&AC_freeFP = dlsym(RTLD_NEXT, "free");
	}
	
	if(AI_CollectionStatus()) {
		AI_StartPacket(AC_moduleID);
		
		*(uint8_t *)AI_PacketSpace(1) = 3;
		*(uint64_t *)AI_PacketSpace(8) = AI_Timestamp();
		*(uint64_t *)AI_PacketSpace(8) = (uint64_t)ptr;
		
		AI_EndPacket();
	}
	
	AC_freeFP(ptr);
}

void AC_EXPORT *realloc(void *ptr, size_t size) {
	if(AC_reallocFP == NULL) {
		*(void **)&AC_reallocFP = dlsym(RTLD_NEXT, "realloc");
	}
	
	void *data = AC_reallocFP(ptr, size);
	if(AI_CollectionStatus()) {
		AI_StartPacket(AC_moduleID);
		
		*(uint8_t *)AI_PacketSpace(1) = 4;
		*(uint64_t *)AI_PacketSpace(8) = AI_Timestamp();
		*(uint64_t *)AI_PacketSpace(8) = (uint64_t)ptr;
		*(uint64_t *)AI_PacketSpace(8) = (uint64_t)data;
		*(uint64_t *)AI_PacketSpace(8) = size;
		
		AI_EndPacket();
	}
	
	return data;
}

void __attribute__((constructor)) AC_EXPORT AM_Construct() {
	AI_Construct();
	
	AC_moduleID = AI_ConfigurationLong("memory:moduleID");
	
	AI_ModuleLoaded("memory", AC_moduleID);
}

void __attribute__((destructor)) AC_EXPORT AM_Destruct() {
	
}
