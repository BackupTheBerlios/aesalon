#include "NetworkReceiver.h"
#include "NetworkReceiver.moc"

NetworkReceiver::NetworkReceiver(QObject* parent, QString host, quint16 port) : DataReceiver(parent), host(host), port(port) {
}

NetworkReceiver::~NetworkReceiver() {
    tcp_socket->deleteLater();
}

void NetworkReceiver::run() {
    tcp_socket = new QTcpSocket(NULL);
    tcp_socket->connectToHost(host, port);
    connect(tcp_socket, SIGNAL(readyRead()), this, SLOT(data_received()));
    connect(tcp_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    exec();
}

void NetworkReceiver::data_received() {
    qDebug("NetworkReceiver: Received data from monitor . . .");
    QByteArray received = tcp_socket->readAll();
    unprocessed += received;
    while(unprocessed.size()) {
        /* TODO: actually process event */
        unprocessed.chop(0);
    }
}


void NetworkReceiver::disconnected() {
    this->exit(0);
    emit no_more_data();
}
