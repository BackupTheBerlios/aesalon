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

#include "common/SharedMemoryHeader.h"
#include "common/Packet.h"

/** Constructor for the Informer module. Should be called from every module constructor.
*/
void __attribute__((constructor)) AI_Construct();
/** Destructor for the Informer module. Should be called from every module destructor.
*/
void __attribute__((destructor)) AI_Destruct();

/** Interally-used function; creates a shared memory block.
*/
void AI_CreateSHM();

/** Sends a packet to the montor via the shared memory block.
	@note If @a packet is NULL, then the corresponding link will terminate.
*/
void AI_SendPacket(Packet *packet);

/** Calculates a unique timestamp for the current instant.
*/
uint64_t AI_Timestamp();

/** Returns a configuration item, @a name, as a string.
*/
const char *AI_ConfigurationString(const char *name);
/** Returns a configuration item, @a name, as a long.
*/
long AI_ConfigurationLong(const char *name);
/** Returns a configuration item, @a name, as a boolean integer.
*/
int AI_ConfigurationBool(const char *name);

struct {
	int fd;
	uint64_t processHash;
	uint8_t *data;
	uint32_t size;
	
	SharedMemoryHeader *header;
} SharedMemory;

#endif
