#ifndef CollectorInterface_H
#define CollectorInterface_H

#include "DataTypes.h"

#if defined(__cplusplus)
extern "C" {
#endif

#if __GNUC__ >= 4
	#define GLOBAL_EXPORT __attribute__((visibility("default")))
	#pragma GCC visibility push(hidden)
#else
	#define GLOBAL_EXPORT
#endif

/* Generic functions. Note that many of these are overloaded by non-implementation
	macros. */
void GLOBAL_EXPORT AesalonCollectorRegisterModule(uint16_t *id);
void GLOBAL_EXPORT AesalonCollectorSendPacket(DataPacket *packet);
void GLOBAL_EXPORT AesalonCollectorFillPacket(DataPacket *packet);
uint64_t GLOBAL_EXPORT AesalonCollectorGetTimestamp();
uint8_t AesalonCollectorCollectionStatus();

/* If AesalonCollectorImplementation is defined, then this header is included
	from within the implementation of the library. */
#ifndef AesalonCollectorImplementation
/* These macros are for global usage by other modules. */

/* AesalonCollectorModule is defined in one -- ONE -- file within the module.
	This is treated as the main file, and variable declarations go in there. */
#ifdef AesalonCollectorModule
	uint16_t AesalonModuleID;
#endif // #ifdef AesalonCollectorModule

#define AesalonCollectorRegisterModule() \
	AesalonCollectorRegisterModule(&AesalonModuleID)

#define AesalonCollectorSendPacket(packet) \
	AesalonCollectorFillPacket(packet); \
	AesalonCollectorSendPacket(packet)

#else
/* These functions/macros are for internal use by the collector interface. */
void __attribute__((constructor)) AesalonCollectorConstructor();
void __attribute__((destructor)) AesalonCollectorDestructor();

struct {
	int fd;
	uint8_t *memory;
	MemoryMapHeader *header;
} AesalonMemoryMap;

#endif // #ifndef AesalonCollectorImplementation

extern uint16_t AesalonModuleID;

#if __GNUC__ >= 4
	#pragma GCC visibility pop
#endif

#if defined(__cplusplus)
} // extern "C"
#endif

#endif
