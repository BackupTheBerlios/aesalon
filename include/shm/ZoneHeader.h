/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/shm/ZoneHeader.h
*/

#ifndef AesalonSHM_ZoneHeader_H
#define AesalonSHM_ZoneHeader_H

#include <semaphore.h>
#include <stdint.h>

#ifdef __cplusplus
namespace SHM {

struct ZoneHeader {
#else
typedef struct ZoneHeader ZoneHeader;
struct ZoneHeader {
#endif
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
	
	/** The size of the end-gap. This is used when a size larger than the remaining space on the end of the
		circular buffer is required.
		
		A zero value indicates either the absence of a gap or the absence of a packet in that region of memory.
		This value should be cleared whenever the gap is read.
	*/
	uint32_t gapSize;
	
	/** The ID of the process that the zone's data comes from. */
	uint32_t processID;
	/** The ID of the thread that the zone's data comes from. */
	uint32_t threadID;
};

#define ZoneDataOffset (sizeof(ZoneHeader)+16)

#ifdef __cplusplus
} // namespace SHM
#endif

#endif
