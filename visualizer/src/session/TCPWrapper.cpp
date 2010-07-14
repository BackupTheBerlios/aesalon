#include "TCPWrapper.h"

TCPWrapper::TCPWrapper(const QString &host, quint16 port) : SessionIOWrapper(), m_host(host), m_port(port) {
	m_tcpSocket = new QTcpSocket(this);
	connect(m_tcpSocket, SIGNAL(connected()), this, SIGNAL(connected()));
	connect(m_tcpSocket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
	connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(disconnected()));
	
	setIODevice(m_tcpSocket);
}

TCPWrapper::~TCPWrapper() {
	
}

void TCPWrapper::open() {
	m_tcpSocket->connectToHost(m_host, m_port);
}
