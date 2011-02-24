/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/monitor/Marshal.h
*/

#ifndef AesalonMonitor_Marshal_H
#define AesalonMonitor_Marshal_H

#include <string>

#include "marshaller/Interface.h"

namespace Monitor {

class Marshal {
private:
	Marshaller::Interface *m_interface;
	void *m_moduleHandle;
public:
	Marshal(const std::string &moduleName);
	Marshal(Marshaller::Interface *interface);
	~Marshal();
	
	Marshaller::Interface *interface() const { return m_interface; }
private:
	void load(const std::string &moduleName);
};

} // namespace Monitor

#endif
