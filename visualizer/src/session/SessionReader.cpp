#include <string.h>

#include "SessionReader.h"
#include "SessionReader.moc"
#include "DataTypes.h"

SessionReader::SessionReader(SessionIOWrapper *ioWrapper) : QObject(NULL), m_ioDevice(ioWrapper->ioDevice()) {
	connect(m_ioDevice, SIGNAL(readyRead()), SLOT(readData()), Qt::QueuedConnection);
}

SessionReader::~SessionReader() {

}

void SessionReader::readData() {
	m_unprocessed += m_ioDevice->readAll();
	DataPacket *packet;
	const uint32_t sourceSize = sizeof(packet->dataSource);
	const uint32_t sizeSize = sizeof(packet->dataSize);
	while(m_unprocessed.size() > int(sourceSize + sizeSize)) {
		packet = new DataPacket;
		memcpy(&packet->dataSource, m_unprocessed.data(), sourceSize);
		memcpy(&packet->dataSize, m_unprocessed.data() + sourceSize, sizeSize);
		if(m_unprocessed.size() < int(packet->dataSize)) {
			delete packet;
			break;
		}
		packet->data = new char[packet->dataSize];
		memcpy(packet->data, m_unprocessed.data() + sourceSize + sizeSize, packet->dataSize);
		/* TODO: push packet onto queue. */
	}
}
