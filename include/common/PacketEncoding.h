/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/PacketEncoding.h

*/

#ifndef AesalonCommon_PacketEncoding_H
#define AesalonCommon_PacketEncoding_H

#include <string.h>

#include "Packet.h"

enum Encodings {
	PE_Uint8 = 0x01,
	PE_Uint16 = 0x02,
	PE_Uint32 = 0x03,
	PE_Uint64 = 0x04,
	PE_String = 0x05
};

#if 0
#define EnsureSpace(PU_packet, PU_size) \
	if(PU_packet->usedSize + PU_size > PU_packet->dataSize) { \
		PU_packet->data = realloc(PU_packet->data, PU_packet->dataSize + 1024);\
		PU_packet->dataSize += 1024; \
	}

#define Packet_AppendType(PU_packet, PU_type) \
	EnsureSpace(PU_packet, 1) \
	PU_packet->data[PU_packet->usedSize] = PU_type; \
	PU_packet->usedSize += 1;

#define Packet_Append(PU_packet, PU_data, PU_size) \
	EnsureSpace(PU_packet, PU_size) \
	memcpy(PU_packet->data + PU_packet->usedSize, PU_data, PU_size); \
	packet->usedSize += PU_size;

#define Packet_Uint8(PU_packet, PU_value) \
	Packet_AppendType(PU_packet, Uint8) \
	Packet_Append(PU_packet, &PU_value, 1)

#define Packet_Uint64(PU_packet, PU_value) \
	Packet_AppendType(PU_packet, Uint64) \
	Packet_Append(PU_packet, &value, 8)
#endif

#endif
