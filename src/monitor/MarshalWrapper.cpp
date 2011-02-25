/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/MarshalWrapper.cpp
*/

#include <dlfcn.h>

#include "monitor/MarshalWrapper.h"
#include "monitor/Coordinator.h"
#include "util/MessageSystem.h"

namespace Monitor {

MarshalWrapper::MarshalWrapper(const std::string &moduleName) : m_interface(NULL), m_handle(NULL) {
	load(moduleName);
}

MarshalWrapper::MarshalWrapper(Marshal::Interface *interface) : m_interface(interface) {
	
}

MarshalWrapper::~MarshalWrapper() {

}

void MarshalWrapper::load(const std::string &moduleName) {
	std::string moduleRoot = Coordinator::instance()->vault()->get(moduleName + ":root");
	std::string marshalPath = Coordinator::instance()->vault()->get(moduleName + ":marshalPath");
	
	m_handle = dlopen((moduleRoot + marshalPath).c_str(), RTLD_LAZY);
	if(m_handle == NULL) {
		Message(Warning, "Could not open marshal library for module \"" << moduleName << "\":" << dlerror());
		return;
	}
	Marshal::Interface *(*instantiate)();
	
	*(void **) (&instantiate) = dlsym(m_handle, "AM_Instantiate");
	
	if(instantiate == NULL) {
		Message(Warning, "Could not find AM_Instantiate in " << moduleName << "'s marshal.");
		return;
	}
	
	m_interface = instantiate();
}

} // namespace Monitor
