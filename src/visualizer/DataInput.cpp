/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/visualizer/DataInput.cpp
*/

#include "visualizer/DataInput.h"
#include "comm/Packet.h"
#include "util/MessageSystem.h"
#include "informer/PacketFormat.h"
#include "visualizer/PIDAllocator.h"

namespace Visualizer {

DataInput::DataInput(ArtisanManager *artisanManager) : m_artisanManager(artisanManager) {
	
}

void DataInput::addData(QByteArray data) {
	Message(Debug, "Data added . . .");
	m_unprocessed += data;
	
	while(m_unprocessed.size() >= int(sizeof(Comm::PacketHeader))) {
		Comm::PacketHeader *header = reinterpret_cast<Comm::PacketHeader *>(m_unprocessed.data());
		if(m_unprocessed.size() < int(sizeof(Comm::PacketHeader) + header->dataSize)) break;
		
		Comm::Packet packet(
			Comm::PacketHeader(
				header->moduleID, m_processIDMap[header->processID], header->threadID, header->dataSize),
			reinterpret_cast<uint8_t *>(m_unprocessed.data()) + sizeof(Comm::PacketHeader));
		
		if(packet.header().moduleID == 0) processInformerPacket(&packet);
		else if(m_artisanMap[packet.header().moduleID] != NULL) {
			Artisan::Interface *interface;
			if((interface = m_artisanMap[packet.header().moduleID]->interface()) != NULL)
				interface->storageObject()->process(&packet);
		}
		/* If there is no artisan loaded for the packet, just ignore it. */
		
		m_unprocessed.remove(0, sizeof(Comm::PacketHeader) + header->dataSize);
	}
}

void DataInput::processInformerPacket(Comm::Packet *packet) {
	Informer::PacketType type = static_cast<Informer::PacketType>(packet->data()[0]);
	
	switch(type) {
		case Informer::ModuleLoaded: {
			Message(Debug, "Loading module . . .");
			ModuleID moduleID = *reinterpret_cast<ModuleID *>(packet->data() + 1);
			std::string name = reinterpret_cast<char *>(packet->data() + 3);
			ArtisanWrapper *artisan = m_artisanManager->artisan(name);
			m_artisanMap[moduleID] = artisan;
			break;
		}
		case Informer::FileLoaded: {
			Message(Warning, "Recieved FileLoaded packet in visualizer -- these are for the monitor only!");
			break;
		}
		case Informer::NewProcess: {
			uint32_t informerID = *reinterpret_cast<uint32_t *>(packet->data() + 1);
			m_processIDMap[informerID] = PIDAllocator::nextID();
			break;
		}
		default: break;
	}
}

} // namespace Visualizer
