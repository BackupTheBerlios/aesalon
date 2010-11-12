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

typedef struct {
	uint32_t size;
} SharedMemoryHeader;

#define SharedMemoryDataOffset (sizeof(SharedMemoryHeader) + 16)

#endif
