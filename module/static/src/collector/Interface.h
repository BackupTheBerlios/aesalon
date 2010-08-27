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
	#define AC_CONSTRUCTOR __attribute__((constructor))
	#define AC_DESTRUCTOR __attribute__((destructor))
	#pragma GCC visibility push(hidden)
#endif

/* These two functions should be overloaded in all modules, but not exported under ANY circumstances. */
void AC_CONSTRUCTOR AC_constructor();
void AC_DESTRUCTOR AC_destructor();

uint16_t AC_EXPORT AC_registerModule(const char *name);

void AC_EXPORT AC_writePacket(AC_DataPacket *packet);
uint64_t AC_EXPORT AC_timestamp();
uint8_t AC_EXPORT AC_hasCollectionBegun();
AC_Address AC_EXPORT AC_libraryOffset(const char *name);

uint16_t AC_moduleID();
void AC_setModuleID(uint16_t moduleID);

#define AC_moduleDefinition \
	static uint16_t AC_m_moduleID; \
	uint16_t AC_moduleID() { return AC_m_moduleID; } \
	void AC_setModuleID(uint16_t moduleID) { AC_m_moduleID = moduleID; }

/*#ifdef AC_GCC
	#pragma GCC visibility pop
#endif*/

#if defined(__cplusplus)
} // extern "C"
#endif

#endif
