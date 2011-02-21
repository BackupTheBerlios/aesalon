/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/shm/PacketHeader.h
*/

#ifndef AesalonSHM_PacketHeader_H
#define AesalonSHM_PacketHeader_H

#include <semaphore.h>
#include <stdint.h>

#include "ModuleID.h"

#ifdef __cplusplus
namespace SHM {

struct PacketHeader {
#else
typedef struct PacketHeader PacketHeader;
struct PacketHeader {
#endif
	/** The size of the packet's data. */
	uint32_t packetSize;
	/** The module ID # of the module that generated this packet. */
	ModuleID moduleID;
};

#ifdef __cplusplus
} // namespace SHM
#endif

#endif
