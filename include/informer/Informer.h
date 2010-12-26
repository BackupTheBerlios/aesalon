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
#include "common/Packet.h"

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

#ifdef AC_INFORMER
/** Interally-used function; creates a shared memory block.
*/
void AC_PRIVATE AI_CreateSHM();
#endif

/** Sends a packet to the montor via a shared memory segment.
	@note If @a packet is NULL, then the corresponding link will terminate.
*/
void AC_EXPORT AI_SendPacket(Packet *packet);

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

/** Returns 1 if data should be collected, 0 otherwise.
*/
short AC_EXPORT AI_CollectionStatus();

/** Stops collection for the current thread ID.
*/
void AC_PRIVATE AI_StopCollection();
/** Continues collection for the current thread ID.
*/
void AC_PRIVATE AI_ContinueCollection();

//#ifdef AC_INFORMER

struct SMS {
	int fd;
	uint8_t *data;
	uint32_t size;
	
	uint32_t smsID;
	
	SharedMemoryHeader *header;
};

struct ThreadListItem {
	pthread_t threadID;
	ThreadListItem *next;
};

struct SMSListItem {
	SMS *sms;
	SMSListItem *next;
};

struct InformerData {
	uint32_t processID;
	
	SMSListItem *smsList;
	int smsListSize;
	
	ThreadListItem *threadList;
	int threadListSize;
	
	
};

//#endif

#ifdef __GNUC__
	#pragma GCC visibility pop
#else
	#error "No supported compiler found."
#endif

#endif
