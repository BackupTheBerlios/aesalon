/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/MarshalWrapper.cpp
*/

#include "monitor/MarshalWrapper.h"
#include "monitor/Coordinator.h"
#include "util/MessageSystem.h"

namespace Monitor {

MarshalWrapper::MarshalWrapper(const std::string &moduleName) : m_interface(NULL), m_moduleHandle(NULL) {
	load(moduleName);
}

MarshalWrapper::MarshalWrapper(Marshal::Interface *interface) : m_interface(interface) {
	
}

MarshalWrapper::~MarshalWrapper() {

}

void MarshalWrapper::load(const std::string &moduleName) {
	Message(Debug, "Looking for module root (config item \"" << moduleName + "::moduleRoot" << "\"");
	std::string moduleRoot = Coordinator::instance()->vault()->get(moduleName + "::moduleRoot");
	std::string marshalPath = Coordinator::instance()->vault()->get(moduleName + "::marshalPath");
}

} // namespace Monitor
