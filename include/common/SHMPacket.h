/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/SHMPacket.h

*/

#ifndef AesalonCommon_SHMPacket_H
#define AesalonCommon_SHMPacket_H

#include "ModuleID.h"

typedef struct SHMPacketHeader_t SHMPacketHeader_t; 
struct SHMPacketHeader_t {
	/** The size of the packet's data.
	*/
	uint32_t packetSize;
	/** The module ID # of the module that generated this packet.
	*/
	ModuleID moduleID;
};

#endif
