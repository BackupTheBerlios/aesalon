/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/Informer.h

*/

#ifndef AesalonInformer_Informer_H
#define AesalonInformer_Informer_H

#include <stdint.h>
#include <pthread.h>

#include "common/SharedMemoryHeader.h"
#include "common/SHMPacket.h"
#include "common/Config.h"

#ifdef __GNUC__
	#pragma GCC visibility push(hidden)
	#define AC_EXPORT __attribute__((visibility("default")))
	#define AC_PRIVATE __attribute__((visibility("hidden")))
#else
	#error "No supported compiler found."
#endif

/** Constructor for the Informer module. Should be called from every module constructor.
*/
void __attribute__((constructor)) AC_EXPORT AI_Construct();
/** Destructor for the Informer module. Should be called from every module destructor.
*/
void __attribute__((destructor)) AC_EXPORT AI_Destruct();

void AC_EXPORT AI_StartPacket(ModuleID moduleID);
void AC_EXPORT *AI_PacketSpace(uint32_t size);
void AC_EXPORT AI_EndPacket();

/** Calculates a unique timestamp for the current instant.
*/
uint64_t AC_EXPORT AI_Timestamp();

/** Returns a configuration item, @a name, as a string.
*/
const char AC_EXPORT *AI_ConfigurationString(const char *name);
/** Returns a configuration item, @a name, as a long.
*/
long AC_EXPORT AI_ConfigurationLong(const char *name);
/** Returns a configuration item, @a name, as a boolean integer.
*/
int AC_EXPORT AI_ConfigurationBool(const char *name);

/** Returns the list of threads currently active for data collection
	in the current process.
	@param size A pointer to an integer to store the size of the list.
	@return The list of target threads, with size @a size.
*/
pthread_t AC_EXPORT *AI_TargetThreadList(int *size);

/** Returns 1 if data should be collected, 0 otherwise.
*/
short AC_EXPORT AI_CollectionStatus();

/** Temporarily pauses collection for the given thread.
*/
void AC_EXPORT AI_StopCollection(pthread_t tid);
/** Continues collection for the given thread.
*/
void AC_EXPORT AI_ContinueCollection(pthread_t tid);

#ifdef __GNUC__
	#pragma GCC visibility pop
#else
	#error "No supported compiler found."
#endif

#endif
