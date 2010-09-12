#include <dlfcn.h>
#include <iostream>

#include "Module.h"
#include "misc/PathSanitizer.h"
#include "misc/StreamAsString.h"
#include "Initializer.h"
#include "LogSystem.h"

namespace Module {

Module::Module(uint16_t moduleID, std::string moduleName) : m_moduleName(moduleName) {
	LogSystem::logModuleMessage(moduleID, Misc::StreamAsString() << "Loading new module: " << moduleName);
	
	std::string modulePath = Misc::PathSanitizer::findFromPaths(
			Initializer::singleton()->configuration()->module(moduleName)->item("polisher")->data(),
			Initializer::singleton()->configuration()->module(moduleName)->item("modulePath")->data());
	
	m_monitorHandle = dlopen(modulePath.c_str(), RTLD_NOW | RTLD_LOCAL);
	if(m_monitorHandle == NULL) {
		LogSystem::logModuleMessage(moduleID, Misc::StreamAsString() << "Couldn't open monitor library: " << dlerror());
		return;
	}
	
	void *instantiationHandle = dlsym(m_monitorHandle, "Instantiate");
	if(instantiationHandle == NULL) {
		LogSystem::logModuleMessage(moduleID, Misc::StreamAsString() << "Polisher library does not have instantiation function.");
		return;
	}
	
	PolisherInterface *(*instantiateFunction)();
	*(void **)(&instantiateFunction) = instantiationHandle;
	m_interface = instantiateFunction();
	m_interface->setAnalyzer(Initializer::singleton()->launcher()->analyzer());
}

Module::~Module() {
	delete m_interface;
	if(m_monitorHandle) dlclose(m_monitorHandle);
}

DataPacket *Module::processPacket(DataPacket *packet) {
	if(m_interface == NULL) return packet;
	else return m_interface->handlePacket(packet);
}

} // namespace Module
