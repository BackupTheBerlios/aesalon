/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/shm/Header.h
*/

#ifndef AesalonSHM_Header_H
#define AesalonSHM_Header_H

#include <semaphore.h>
#include <stdint.h>

#ifdef __cplusplus
namespace SHM {

struct Header {
#else
typedef struct SHMHeader SHMHeader;
struct SHMHeader {
#endif
	/** The overall size of the SHM in pages. */
	uint32_t shmSize;

	/** The size of the configuration data (in pages). */
	uint32_t configDataSize;
	
	/** The number of pages to use to store zone usage data in. */
	uint32_t zoneUsagePages;
	/** The page offset that zones are created starting at. */
	uint32_t zonePageOffset;
	/** The size of a zone, in pages. */
	uint32_t zoneSize;
	/** The number of zones currently in use. */
	uint32_t zoneCount;
	/** The number of zones that memory has been allocated for. */
	uint32_t zonesAllocated;
	
	/** Packet semaphore: incremented when a packet is available for reading. */
	sem_t packetSemaphore;
	/** Resizing semaphore; locked when a thread is resizing the SHM. */
	sem_t resizeSemaphore;
	
	/** The last allocated process ID; used to assign 4-byte process IDs dynamically. */
	uint32_t lastProcessID;
	/** The last allocated thread ID; used to assign 4-byte thread IDs dynamically. */
	uint32_t lastThreadID;
	
	/** The number of running, monitored processes. Used to exit the monitor when required. */
	uint16_t runningProcesses;
};

#ifdef __cplusplus
} // namespace SHM
#endif

#endif
