#ifndef DataTypes_H
#define DataTypes_H

#include <stdint.h>

/** Structure used to transfer data from collectors to the monitor. */
typedef struct {
	/** Module ID is filled in automatically by the collector interface. */
	uint16_t moduleID;
	/** Source information. */
	struct {
		/** The timestamp this collection occurred at. */
		uint64_t timestamp;
		/** The thread the event occurred in. */
		int64_t thread;
	} dataSource;
	/** The data to send. */
	void *data;
	/** The size of the data. */
	uint32_t dataSize;
} DataPacket;

#endif
