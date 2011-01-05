/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/ZoneHeader.h

*/

#ifndef AesalonCommon_ZoneHeader_H
#define AesalonCommon_ZoneHeader_H

#include <stdint.h>
#include <semaphore.h>
#include <sys/types.h>

#define ZoneDataOffset (sizeof(ZoneHeader_t) + 16)

typedef struct ZoneHeader_t ZoneHeader_t;
struct ZoneHeader_t {
	
	/** Head element of the data region for the circular buffer. */
	uint32_t head;
	
	/** Tail element of the data region for the circular buffer. */
	uint32_t tail;
	
	/** Packet semaphore: incremented when a packet is available for reading. */
	sem_t packetSemaphore;
	
	/** Overflow value: if set to a non-zero value, then the value is how many more bytes are
		required before the next packet can be written. When this value reaches < 0, @a overflowSemaphore
		should be incremented.
	*/
	int32_t overflow;
	/** The overflow semaphore, see @a overflow for description. */
	sem_t overflowSemaphore;
	
	/** The PID that the zone's data comes from. */
	pid_t processID;
	
	/** The thread ID the zone's data comes from. */
	pthread_t threadID;
};

#endif
