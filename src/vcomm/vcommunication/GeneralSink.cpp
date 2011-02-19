/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/vcommunication/GeneralSink.cpp

*/

#include "vcommunication/GeneralSink.h"

namespace Monitor {
namespace VCommunication {

GeneralSink::GeneralSink() {
	
}

GeneralSink::~GeneralSink() {
	
}

void GeneralSink::addDataSink(DataSink *dataSink) {
	m_dataSinkList.push_back(dataSink);
}

void GeneralSink::sinkPacket(Common::VPacket *packet) {
	for(DataSinkList::const_iterator i = m_dataSinkList.begin(); i != m_dataSinkList.end(); ++i) {
		(*i)->sinkPacket(packet);
	}
}

} // namespace VCommunication
} // namespace Monitor
