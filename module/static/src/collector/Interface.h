#ifndef CollectorInterface_H
#define CollectorInterface_H

#include <stdint.h>

#include "DataTypes.h"

#if defined(__cplusplus)
extern "C" {
#endif

#ifdef __GNUC__
	#define AC_GCC
#else
	#error "No supported compiler found."
#endif

#ifdef AC_GCC
	#define AC_EXPORT __attribute__((visibility("default")))
	#pragma GCC visibility push(hidden)
#endif

typedef struct {
	const char *name;
	uint16_t id;
	void *modulePtr;
} AC_Module;

typedef struct {
	/* Module data. */
	int mmap_fd;
	void *memory;
	AC_MemoryMapHeader *header;
	
	/* Module API */
	void (*sendPacket)(AC_DataPacket *packet);
	uint64_t (*getTimestamp)();
	uint8_t (*status)();
} AC_Interface;

/* These two functions should be overloaded in all modules, but not exported under ANY circumstances. */
void __attribute__((constructor)) AC_Constructor();
void __attribute__((destructor)) AC_Destructor();

AC_Interface AC_EXPORT *AC_GetInterface();
void AC_EXPORT *AC_GetModule(const char *name);
void AC_EXPORT AC_RegisterModule(AC_Module *module);

#ifdef AC_GCC
	#pragma GCC visibility pop
#endif

#if defined(__cplusplus)
} // extern "C"
#endif

#endif
