#include <iostream>
#include <QTimer>
#include "ActiveSessionSocket.h"
#include "ActiveSessionSocket.moc"




ActiveSessionSocket::ActiveSessionSocket(QString host, int port) :host(host), port(port) {
    socket = new QTcpSocket();
    
    socket->connectToHost(host, port);
    connect(socket, SIGNAL(readyRead()), this, SLOT(handle_data()));
    connect(socket, SIGNAL(connected()), this, SLOT(reemit_connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(reemit_disconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error_caught(QAbstractSocket::SocketError)));
}

ActiveSessionSocket::~ActiveSessionSocket() {
}

void ActiveSessionSocket::handle_data() {
    QByteArray data = socket->readAll();
    emit received_data(data);
}

void ActiveSessionSocket::error_caught(QAbstractSocket::SocketError error) {
    /* if it's a timeout or connection-refused, try reconnecting in 500ms . . . */
    if(error == QAbstractSocket::SocketTimeoutError) {
        QTimer::singleShot(500, this, SLOT(try_connecting()));
    }
    else if(error == QAbstractSocket::ConnectionRefusedError) {
        QTimer::singleShot(500, this, SLOT(try_connecting()));
    }
}

void ActiveSessionSocket::try_connecting() {
    socket->connectToHost(host, port);
}



