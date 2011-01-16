/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/visualizer/communication/LogSource.h

*/

#ifndef AesalonVisualizer_Communication_LogSource_H
#define AesalonVisualizer_Communication_LogSource_H

#include <QString>
#include <QFile>

#include "DataSource.h"

namespace Visualizer {
namespace Communication {

class LogSource : public DataSource {
private:
	QFile *m_file;
	QByteArray m_data;
	Common::VPacket m_packet;
public:
	LogSource(const QString &filename);
	virtual ~LogSource();
	
	virtual bool open();
	
	virtual Common::VPacket *nextPacket();
};

} // namespace Communication
} // namespace Visualizer

#endif
