/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file informer/Informer.h
*/

#ifndef AesalonInformer_Informer_H
#define AesalonInformer_Informer_H

#include <stdint.h>
#include <pthread.h>

#include "ModuleID.h"
#include "shm/Header.h"
#include "shm/ZoneHeader.h"

#ifdef __GNUC__
	#pragma GCC visibility push(hidden)
	#define AC_EXPORT __attribute__((visibility("default")))
	#define AC_PRIVATE __attribute__((visibility("hidden")))
	
	#define THREAD_SPECIFIC __thread
#else
	#error "No supported compiler detected."
#endif

/* Constructor/destructors. */
/** Constructor for the Informer module. Should be called from every module constructor. */
void __attribute__((constructor)) AC_EXPORT AI_Construct();
/** Destructor for the Informer module. Should be called from every module destructor. */
void __attribute__((destructor)) AC_EXPORT AI_Destruct();

/* Packet functions. */
void AC_EXPORT AI_StartPacket(ModuleID moduleID);
void AC_EXPORT *AI_PacketSpace(uint32_t size);
void AC_EXPORT AI_EndPacket();

/* Utility functions. */
/** Calculates a unique timestamp for the current instant. */
uint64_t AC_EXPORT AI_Timestamp();
void AC_EXPORT AI_ModuleLoaded(const char *name, ModuleID moduleID);

/* Configuration functions. */
/** Returns a configuration item, @a name, as a string. */
const char AC_EXPORT *AI_ConfigurationString(const char *name);
/** Returns a configuration item, @a name, as a long. */
long AC_EXPORT AI_ConfigurationLong(const char *name);
/** Returns a configuration item, @a name, as a boolean integer. */
int AC_EXPORT AI_ConfigurationBool(const char *name);

/* Collection management functions. */
/** Temporarily pauses collection for the given thread ID. */
void AC_EXPORT AI_StopCollection(pthread_t threadID);
/** Continues collection for the given thread. */
void AC_EXPORT AI_ContinueCollection(pthread_t threadID);
/** Returns 1 if data should be collected, 0 otherwise. */
short AC_EXPORT AI_CollectionStatus();

#endif
