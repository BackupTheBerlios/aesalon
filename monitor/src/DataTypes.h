#ifndef DataTypes_H
#define DataTypes_H

#include <stdint.h>

typedef unsigned long Address;

/** Structure used to transfer data from collectors to the monitor. */
typedef struct {
	/** Module ID is filled in automatically by the collector interface. */
	uint16_t moduleID;
	/** Source information, also automatically filled in. */
	struct {
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
	/** The latest-loaded collection module. */
	uint16_t latestModule;
	/** Futex controlling latestModule. */
	int latestModuleFutex;
	
	/** 0 if the program is still initializing, 1 if main() has been reached. */
	uint8_t isMainReached;
	
	/** The size of the memory map. */
	uint64_t dataSize;
	
	/** The futex used to control data packet access without infinite sleep() loops. */
	int dataFutex;
	/** The offset at which the unprocessed data begins. Incremented by the monitor. */
	uint64_t dataStart;
	/** Futex (used as a mutex) controlling dataStart. */
	int dataStartFutex;
	/** The offset at which the unprocessed data ends. Incremented by the collection interface. */
	uint64_t dataEnd;
	/** Futex controlling dataEnd. */
	int dataEndFutex;
} MemoryMapHeader;

#endif
