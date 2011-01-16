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

LogSource::LogSource(const QString &filename) {
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
	
	return NULL;
}

} // namespace Communication
} // namespace Visualizer
