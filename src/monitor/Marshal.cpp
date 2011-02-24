/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/Marshal.cpp
*/

#include "monitor/Marshal.h"



namespace Monitor {

Marshal::Marshal(const std::string &moduleName) : m_interface(NULL), m_moduleHandle(NULL) {
	
}

Marshal::Marshal(Marshaller::Interface *interface) : m_interface(interface) {
	
}

Marshal::~Marshal() {

}

void Marshal::load(const std::string &moduleName) {
	
}

} // namespace Monitor
