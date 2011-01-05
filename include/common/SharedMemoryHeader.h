/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/SharedMemoryHeader.h

*/

#ifndef AesalonCommon_SharedMemoryHeader_H
#define AesalonCommon_SharedMemoryHeader_H

#include <stdint.h>
#include <semaphore.h>

typedef struct SharedMemoryHeader_t SharedMemoryHeader_t;
struct SharedMemoryHeader_t {
	/** Overall size of SHM in pages. */
	uint32_t shmSize;
	
	/** The size of the configuration data (in bytes). */
	uint32_t configDataSize;
	
	/** The number of pages to use to store zone usage data in. */
	uint32_t zoneUsagePages;
	
	/** The page offset that zones are created starting at. */
	uint32_t zonePageOffset;
	
	/** The size of a zone, in pages. */
	uint32_t zoneSize;
	
	/** The number of zones currently in use. */
	uint32_t zoneCount;
	
	/** Packet semaphore: incremented when a packet is available for reading. */
	sem_t packetSemaphore;
};

#endif
