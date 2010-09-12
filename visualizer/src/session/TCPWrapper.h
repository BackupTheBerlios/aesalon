#ifndef TCPWrapper_H
#define TCPWrapper_H

#include <QTcpSocket>

#include "SessionIOWrapper.h"

class TCPWrapper : public SessionIOWrapper {
public:
	TCPWrapper(const QString &host, quint16 port);
	virtual ~TCPWrapper();
private:
	QString m_host;
	quint16 m_port;
	QTcpSocket *m_tcpSocket;
public:
	virtual void open();
	
	virtual bool hasHeartbeat() { return true; }
};

#endif
