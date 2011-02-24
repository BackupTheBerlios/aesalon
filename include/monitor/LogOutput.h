/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/monitor/LogOutput.h
*/

#ifndef AesalonMonitor_LogOutput_H
#define AesalonMonitor_LogOutput_H

#include <string>

#include "DataOutput.h"

namespace Monitor {

class LogOutput : public DataOutput {
private:
	int m_fd;
public:
	LogOutput(std::string spec);
	virtual ~LogOutput();
	
	virtual void output(Comm::Packet *packet);
};

} // namespace Monitor

#endif
