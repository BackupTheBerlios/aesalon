#ifndef DataTypes_H
#define DataTypes_H

#include <stdint.h>

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
	uint16_t latestModule;
	int latestModuleFutex;
	
	uint64_t dataSize;
	uint64_t dataStart;
	int dataStartFutex;
	uint64_t dataEnd;
	int dataEndFutex;
	
} MemoryMapHeader;

#endif
