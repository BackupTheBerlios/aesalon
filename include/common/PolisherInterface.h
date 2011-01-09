/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/PolisherInterface.h

*/

#ifndef AesalonCommon_PolisherInterface_H
#define AesalonCommon_PolisherInterface_H

#include "SHMPacket.h"
#include "NetworkPacket.h"

namespace Common {

class PolisherInterface {
public:
	virtual ~PolisherInterface() {}
	
	virtual void polish(SHMPacketHeader_t *header, void *data) = 0;
};

} // namespace Common

extern "C" {

Common::PolisherInterface *AM_InstantiatePolisher();

} // extern "C"

#endif
