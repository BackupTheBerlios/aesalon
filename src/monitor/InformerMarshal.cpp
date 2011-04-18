/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/InformerMarshal.cpp
*/

#include "monitor/InformerMarshal.h"
#include "util/MessageSystem.h"
#include "informer/PacketFormat.h"
#include "monitor/MarshalList.h"
#include "monitor/Coordinator.h"
#include "config/GlobalVault.h"
#include "util/StringTo.h"

namespace Monitor {

InformerMarshal::InformerMarshal() {

}

InformerMarshal::~InformerMarshal() {

}

Comm::Packet *InformerMarshal::marshal(Comm::Packet *packet) {
	Informer::PacketType type = static_cast<Informer::PacketType>(packet->data()[0]);
	
	Message(Debug, "type: " << type);
	
	switch(type) {
		case Informer::ModuleLoaded: {
			moduleLoaded(packet);
			break;
		}
		case Informer::FileLoaded: {
			fileLoaded(packet);
			packet = NULL;
			break;
		}
		case Informer::NewProcess: {
			Message(Log, "Monitoring new process.");
			break;
		}
		case Informer::NewThread: {
			Message(Fatal, "Informer::NewThread handling NYI.");
			break;
		}
		case Informer::ThreadExiting: {
			Message(Fatal, "Informer::ThreadExiting handling NYI.");
			break;
		}
		case Informer::ProcessExiting: {
			Message(Log, "Process exited.");
			break;
		}
	}
	
	return packet;
}

void InformerMarshal::moduleLoaded(Comm::Packet *packet) {
	/* NOTE: the +3 is +1 for the type byte, +2 for the module ID#. (unused in the monitor) */
	std::string name = reinterpret_cast<char *>(packet->data() + 3);
	MarshalList *list = Coordinator::instance()->marshalList();
	
	Message(Debug, "Marshal name: \"" << name << "\"");
	
	list->loadMarshal(name);
}

void InformerMarshal::fileLoaded(Comm::Packet *packet) {
	uint64_t baseAddress = *reinterpret_cast<uint64_t *>(packet->data() + 1);
	uint64_t fileOffset = *reinterpret_cast<uint64_t *>(packet->data() + 9);
	std::string name = reinterpret_cast<char *>(packet->data() + 17);
	
	Coordinator::instance()->resolver()->parse(name, baseAddress);
}

} // namespace Monitor
