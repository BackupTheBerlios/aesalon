#include "TCPWrapper.h"

TCPWrapper::TCPWrapper(const QString &host, quint16 port) : SessionIOWrapper() {
	m_tcpSocket = new QTcpSocket(this);
	m_tcpSocket->connectToHost(host, port);
	connect(m_tcpSocket, SIGNAL(connected()), this, SIGNAL(connected()));
	connect(m_tcpSocket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
	connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(disconnected()));
	
	setIODevice(m_tcpSocket);
}

TCPWrapper::~TCPWrapper() {
	
}
