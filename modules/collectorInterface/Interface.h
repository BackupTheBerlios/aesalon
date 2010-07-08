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

/* If AesalonCollectorImplementation is defined, then this header is included
	from within the implementation of the library. */
#ifndef AesalonCollectorImplementation
/* These functions and macros are for global usage by other modules. */

/* Macros */

/* AesalonCollectorModule is defined in one -- ONE -- file within the module.
	This is treated as the main file, and variable declarations go in there. */
#ifdef AesalonCollectorModule
	uint16_t AesalonModuleID;
#endif // #ifdef AesalonCollectorModule

#define AesalonCollectorPacketFill(packet) \
	((packet)->moduleID = AesalonModuleID); \

/* Functions */
void GLOBAL_EXPORT AesalonCollectorRegisterModule();
void GLOBAL_EXPORT AesalonCollectorSendPacket(DataPacket *packet);

#endif // #ifndef AesalonCollectorImplementation


#if __GNUC__ >= 4
	#pragma GCC visibility pop
#endif

#if defined(__cplusplus)
} // extern "C"
#endif

#endif
