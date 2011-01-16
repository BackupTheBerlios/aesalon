/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file visualizer/src/communication/LogSource.cpp

*/

#include "communication/LogSource.h"

namespace Visualizer {
namespace Communication {

LogSource::LogSource(const QString &filename) : m_packet(0, 0, 0, NULL, 0) {
	m_file = new QFile(filename);
}

LogSource::~LogSource() {
	m_file->close();
}

bool LogSource::open() {
	return m_file->open(QIODevice::ReadOnly);
}

Common::VPacket *LogSource::nextPacket() {
	ModuleID moduleID;
	uint32_t pid, tid, dataSize;
	
	m_data = m_file->read(sizeof(moduleID) + sizeof(pid)*3);
	moduleID = *reinterpret_cast<ModuleID *>(m_data.data());
	pid = *reinterpret_cast<uint32_t *>(m_data.data() + sizeof(moduleID));
	tid = *reinterpret_cast<uint32_t *>(m_data.data() + sizeof(moduleID) + sizeof(pid));
	dataSize = *reinterpret_cast<uint32_t *>(m_data.data() + sizeof(moduleID) + sizeof(pid)*2);
	
	m_data = m_file->read(dataSize);
	
	m_packet = Common::VPacket(pid, tid, moduleID, m_data.data(), dataSize);
	
	return &m_packet;
}

} // namespace Communication
} // namespace Visualizer
