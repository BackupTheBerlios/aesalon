/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/Packet.h

*/

#ifndef AesalonCommon_Packet_H
#define AesalonCommon_Packet_H

#include <stdint.h>

typedef struct {
	/** The process hash is a value that is automatically generated by the
		Informer based on values relatively unique to the process. This field
		will automatically be filled in by AI_SendPacket.
	*/
	uint64_t processHash;
	
	/** The allocated size of the data element. */
	uint32_t dataSize;
	/** The actual data itself. */
	uint8_t *data;
	/** The amount of data that has actually been used. (Applied internally in
		doubling-behaviour inside the Informer module.) */
	uint32_t usedSize;
} Packet;

#endif
