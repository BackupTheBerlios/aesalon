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
/** Registers a module with the monitor, assigning it a module ID so it can send data.
	@param moduleName The name of the module, used to load the corresponding monitor library.
	@param id A pointer to the global variable AesalonModuleID; automatically provided by the
		macro with the same name as the function.
*/
void GLOBAL_EXPORT AesalonCollectorRegisterModule(const char *moduleName, uint16_t *id);
/** Sends a packet to the monitor. @a packet will be sent almost verbatim to the monitor
	library.
	@param packet The packet to send.
*/
void GLOBAL_EXPORT AesalonCollectorSendPacket(DataPacket *packet);
uint64_t GLOBAL_EXPORT AesalonCollectorGetTimestamp();
uint8_t GLOBAL_EXPORT AesalonCollectionStatus();

/* If AesalonCollectorImplementation is defined, then this header is included
	from within the implementation of the library. */
#ifndef AesalonCollectorImplementation
/* These macros are for global usage by other modules. */

/* AesalonCollectorModule is defined in one -- ONE -- file within the module.
	This is treated as the main file, and variable declarations go in there. */
#ifdef AesalonCollectorModule
	uint16_t AesalonModuleID;
#endif // #ifdef AesalonCollectorModule

#define AesalonCollectorRegisterModule(moduleName) \
	AesalonCollectorRegisterModule((moduleName), &AesalonModuleID)

#define AesalonCollectorSendPacket(packet) \
	(packet)->dataSource.moduleID = AesalonModuleID; \
	AesalonCollectorSendPacket(packet)

#else
/* These functions/macros are for internal use by the collector interface. */
void __attribute__((constructor)) AesalonCollectorConstructor();
void __attribute__((destructor)) AesalonCollectorDestructor();

/** Calculates the remaining data space inside the memory map.
	@return The available space for data.
*/
int AesalonCollectorRemainingSpace();
/** Writes @a data into the memory map. Assumes there is at least @a size bytes free.
	@param data The data to write.
	@param size The size, in bytes, of @a data.
*/
void AesalonCollectorWriteData(void *data, int size);
/** Fills a packet with information about the current environment. Does not create a backtrace.
	@param packet The packet to fill.
*/
void AesalonCollectorFillPacket(DataPacket *packet);

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
