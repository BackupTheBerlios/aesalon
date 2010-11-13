/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/ConductorPacket.h

*/

#ifndef AesalonCommon_ConductorPacket_H
#define AesalonCommon_ConductorPacket_H

#include <stdint.h>

enum {
	ConductorPacket_NewProcess,
	ConductorPacket_ModuleLoaded
};

#endif
