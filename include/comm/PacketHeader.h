/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/comm/PacketHeader.h
*/

#ifndef AesalonComm_PacketHeader_H
#define AesalonComm_PacketHeader_H

#include <stdint.h>
#include "ModuleID.h"

namespace Comm {

struct PacketHeader {
	ModuleID moduleID;
	uint32_t processID;
	uint32_t threadID;
	uint32_t dataSize;
	
	/** Convience constructor. */
	PacketHeader(ModuleID moduleID, uint32_t processID, uint32_t threadID, uint32_t dataSize)
		: moduleID(moduleID), processID(processID), threadID(threadID), dataSize(dataSize) {}
};

} // namespace Comm

#endif
