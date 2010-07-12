#include <iostream>
#include "Reader.h"
#include "Initializer.h"
#include "LogSystem.h"

namespace Module {

Reader::Reader() {
	m_mapper = Initializer::singleton()->moduleMapper();
	m_socketManager = Initializer::singleton()->socketManager();
}

Reader::~Reader() {

}

void Reader::processPacket(DataPacket *packet) {
	if(packet->dataSource.moduleID != 0) {
		/* Handle it the normal way. */
		packet = m_mapper->module(packet->dataSource.moduleID)->processPacket(packet);
	}
	else {
		char *moduleName = (char *)packet->data;
		m_mapper->loadModule(moduleName);
	}
	m_socketManager->sendPacket(packet);
}

} // namespace Module
