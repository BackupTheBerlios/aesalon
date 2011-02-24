/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/monitor/MarshalWrapper.h
*/

#ifndef AesalonMonitor_MarshalWrapper_H
#define AesalonMonitor_MarshalWrapper_H

#include <string>

#include "marshal/Interface.h"

namespace Monitor {

class MarshalWrapper {
private:
	Marshal::Interface *m_interface;
	void *m_moduleHandle;
public:
	MarshalWrapper(const std::string &moduleName);
	MarshalWrapper(Marshal::Interface *interface);
	~MarshalWrapper();
	
	Marshal::Interface *interface() const { return m_interface; }
private:
	void load(const std::string &moduleName);
};

} // namespace Monitor

#endif
