#ifndef TCPWrapper_H
#define TCPWrapper_H

#include <QTcpSocket>

#include "SessionIOWrapper.h"

class TCPWrapper : public SessionIOWrapper {
public:
	TCPWrapper(const QString &host, quint16 port);
	virtual ~TCPWrapper();
private:
	QTcpSocket *m_tcpSocket;
public:
	
};

#endif
