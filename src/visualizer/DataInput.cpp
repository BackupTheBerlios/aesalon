/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/visualizer/DataInput.cpp
*/

#include "visualizer/DataInput.h"

#include "comm/Packet.h"

namespace Visualizer {

void DataInput::addData(QByteArray &data) {
	m_unprocessed += data;
	
	while(m_unprocessed.size() >= sizeof(Comm::PacketHeader)) {
		Comm::PacketHeader *header = reinterpret_cast<Comm::PacketHeader *>(m_unprocessed.data());
		if(m_unprocessed.size() < sizeof(Comm::PacketHeader) + header->dataSize) break;
		
		
	}
}

} // namespace Visualizer
