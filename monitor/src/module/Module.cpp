#include <dlfcn.h>
#include <iostream>

#include "Module.h"
#include "misc/PathSanitizer.h"
#include "misc/StreamAsString.h"
#include "Initializer.h"
#include "LogSystem.h"
#include "misc/ConfigurationItemString.h"

namespace Module {

Module::Module(uint16_t moduleID, std::string moduleName) : m_moduleName(moduleName) {
	LogSystem::logModuleMessage(moduleID, Misc::StreamAsString() << "Loading new module: " << moduleName);
	
	std::string modulePath = Misc::PathSanitizer::findFromPaths(moduleName, Initializer::singleton()->configuration()->configItems()["search-path"]->stringValue());
	
	Initializer::singleton()->configuration()->processConfigFile(Misc::StreamAsString() << modulePath << "/monitor.conf");
	
	
	
	/*std::string moduleFilename = "lib";
	moduleFilename += moduleName;
	moduleFilename += "Monitor.so";
	std::string modulePath = Misc::PathSanitizer::findFromPaths(moduleFilename, Initializer::singleton()->configuration()->configItems()["search-path"]->stringValue());
	
	m_monitorHandle = dlopen(modulePath.c_str(), RTLD_NOW | RTLD_LOCAL);
	if(m_monitorHandle == NULL) {
		LogSystem::logModuleMessage(moduleID, Misc::StreamAsString() << "Couldn't open monitor library: " << dlerror());
		return;
	}
	void *instantiationHandle = dlsym(m_monitorHandle, "AesalonMonitorCreateInstance");
	if(instantiationHandle == NULL) {
		LogSystem::logModuleMessage(moduleID, Misc::StreamAsString() << "Monitor library does not have instantiation function.");
		return;
	}
	
	MonitorInterface *(*instantiateFunction)();
	*(void **)(&instantiateFunction) = instantiationHandle;
	m_interface = instantiateFunction();
	m_interface->setAnalyzer(Initializer::singleton()->launcher()->analyzer());*/
}

Module::~Module() {
	delete m_interface;
}

DataPacket *Module::processPacket(DataPacket *packet) {
	if(m_interface == NULL) return packet;
	else return m_interface->handlePacket(packet);
}

} // namespace Module
