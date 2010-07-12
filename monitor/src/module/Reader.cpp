#include <iostream>
#include "Reader.h"
#include "Initializer.h"
#include "LogSystem.h"

namespace Module {

Reader::Reader() {
	m_mapper = Initializer::singleton()->moduleMapper();
}

Reader::~Reader() {

}

void Reader::processPacket(DataPacket *packet) {
	if(packet->dataSource.moduleID != 0) {
		/* Handle it the normal way. */
		m_mapper->module(packet->dataSource.moduleID)->processPacket(packet);
	}
	else {
		char *moduleName = (char *)packet->data;
		std::cout << "New module registered, name is " << moduleName << " . . ." << std::endl;
		m_mapper->loadModule(moduleName);
	}
}

} // namespace Module
