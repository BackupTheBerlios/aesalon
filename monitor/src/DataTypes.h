#ifndef DataTypes_H
#define DataTypes_H

#include <stdint.h>
#include <semaphore.h>

#ifdef AesalonCollector
	#define Address AC_Address
	#define DataPacket AC_DataPacket
	#define MemoryMapHeader AC_MemoryMapHeader
#endif

typedef unsigned long Address;

/** Structure used to transfer data from collectors to the monitor. */
typedef struct {
	/** Source information, automatically filled in. */
	struct {
		/** Module ID this packet was sent from. */
		uint16_t moduleID;
		/** The timestamp this collection occurred at. */
		uint64_t timestamp;
		/** The thread the event occurred in. */
		int64_t thread;
	} dataSource;
	/** The data this packet contains. Dynamically-allocated or otherwise. */
	void *data;
	/** The size of the data. Note that the assumption that the data packet
		is less than 4GB is made here. */
	uint32_t dataSize;
} DataPacket;

/** Structure used for maintaining a header within the SHM used to transfer
	data between the collector interface and the monitor. */
typedef struct {
	/** The latest-loaded collection module ID. */
	uint16_t latestModule;
	
	/** 0 if the program is still initializing, 1 if main() has been reached. */
	uint8_t mainReached;
	/** 0 if data is still being generated, 1 if all child processes have terminated. */
	uint8_t finished;
	
	/** The size of the memory map. */
	uint64_t dataSize;
	/** Data offset; how far from the beginning of the memory map the data begins. */
	uint64_t dataOffset;
	
	/** The semaphore used to control data packet access without infinite sleep() loops. */
	sem_t dataSempahore;
	/** The overflow semaphore, used to control access when the monitor is processing too slowly. */
	sem_t dataOverflowSemaphore;
	/** The overflow flag. When this is true, then the overflow semaphore is used. */
	uint8_t dataOverflow;
	
	/** The offset at which the unprocessed data begins. Incremented by the monitor. */
	uint64_t dataStart;
	/** Semaphore (used as a mutex) controlling dataStart. */
	sem_t dataStartSemaphore;
	/** The offset at which the unprocessed data ends. Incremented by the collection interface. */
	uint64_t dataEnd;
	/** Semaphore controlling dataEnd. */
	sem_t dataEndSemaphore;
} MemoryMapHeader;

#ifdef AesalonCollector
	#undef Address
	#undef DataPacket
	#undef MemoryMapHeader
#endif

#endif
