/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file visualizer/src/communication/NetworkSource.cpp

*/

#include "communication/NetworkSource.h"

namespace Visualizer {
namespace Communication {

NetworkSource::NetworkSource(const QString &hostname, quint16 port) {
	m_socket = new QTcpSocket();
	m_socket->connectToHost(hostname, port);
}

NetworkSource::~NetworkSource() {

}

bool NetworkSource::open() {
	return m_socket->waitForConnected(30000);
}

Common::VPacket *NetworkSource::nextPacket() {
	return NULL;
}

} // namespace Communication
} // namespace Visualizer
