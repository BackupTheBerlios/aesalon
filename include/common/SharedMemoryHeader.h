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

typedef struct {
	uint32_t size;
	
	sem_t packetSemaphore;
	
	uint32_t dataStart;
	uint32_t dataEnd;
	sem_t endSemaphore;
} SharedMemoryHeader;

#define SharedMemoryDataOffset (sizeof(SharedMemoryHeader) + 16)

#endif
