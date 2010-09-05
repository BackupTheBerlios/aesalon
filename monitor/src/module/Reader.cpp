#include <iostream>
#include "Reader.h"
#include "Initializer.h"
#include "LogSystem.h"
#include "misc/StreamAsString.h"

namespace Module {

Reader::Reader() {
	m_mapper = Initializer::singleton()->moduleMapper();
	m_socketManager = Initializer::singleton()->socketManager();
	m_logger = new Program::Logger();
}

Reader::~Reader() {
	delete m_logger;
}

void Reader::processPacket(DataPacket *packet) {
	if(packet->dataSource.moduleID != 0) {
		/* Handle it the normal way. */
		Module::Module *module = m_mapper->module(packet->dataSource.moduleID);
		if(module != NULL) packet = module->processPacket(packet);
		else {
			LogSystem::logReaderMessage(Misc::StreamAsString() << "Received packet for unknown module ID #" << packet->dataSource.moduleID);
		}
	}
	else {
		if(packet->dataSize) {
			char *moduleName = (char *)packet->data;
			m_mapper->loadModule(moduleName);
		}
	}
	m_logger->logPacket(packet);
	m_socketManager->sendPacket(packet);
	
	delete[] (char *)packet->data;
	
	delete packet;
}

} // namespace Module
