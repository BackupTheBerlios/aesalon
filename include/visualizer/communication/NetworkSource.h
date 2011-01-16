/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/visualizer/communication/NetworkSource.h

*/

#ifndef AesalonVisualizer_Communication_NetworkSource_H
#define AesalonVisualizer_Communication_NetworkSource_H

#include <QString>
#include <QTcpSocket>

#include "DataSource.h"

namespace Visualizer {
namespace Communication {

class NetworkSource : public DataSource {
private:
	QTcpSocket *m_socket;
public:
	NetworkSource(const QString &hostname, quint16 port);
	virtual ~NetworkSource();
	
	virtual bool open();
	
	virtual Common::VPacket *nextPacket();
};

} // namespace Communication
} // namespace Visualizer

#endif
