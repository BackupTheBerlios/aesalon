/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/comm/Packet.h
*/

#ifndef AesalonComm_Packet_H
#define AesalonComm_Packet_H

#include "PacketHeader.h"

namespace Comm {

class Packet {
private:
	PacketHeader m_header;
	uint8_t *m_data;
public:
	Packet(PacketHeader header, uint8_t *data)
		: m_header(header), m_data(data) {}
	
	const PacketHeader &header() const { return m_header; }
	void setDataSize(uint32_t newSize) { m_header.dataSize = newSize; }
	uint8_t *data() const { return m_data; }
};

} // namespace Comm

#endif
