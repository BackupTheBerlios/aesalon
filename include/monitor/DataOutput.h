/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/monitor/DataOutput.h
*/

#ifndef AesalonMonitor_DataOutput_H
#define AesalonMonitor_DataOutput_H

#include "comm/Packet.h"

namespace Monitor {

class DataOutput {
public:
	virtual ~DataOutput() {}
	
	virtual void output(Comm::Packet *packet) = 0;
};

} // namespace Monitor

#endif
