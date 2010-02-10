#include "NetworkReceiver.h"
#include "NetworkReceiver.moc"

NetworkReceiver::NetworkReceiver(QObject* parent, QString host, quint16 port) : DataReceiver(parent), host(host), port(port) {
    qDebug("NetworkReceiver::NetworkReceiver(): called . . .");
}

NetworkReceiver::~NetworkReceiver() {
}

void NetworkReceiver::run() {
    QTcpSocket socket(NULL);
    tcp_socket = &socket;
    tcp_socket->connectToHost(host, port);
    connect(tcp_socket, SIGNAL(readyRead()), this, SLOT(data_received()));
    connect(tcp_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    /* Start the event loop . . . */
    exec();
}

void NetworkReceiver::data_received() {
    qDebug("NetworkReceiver: Received data from monitor . . .");
    QByteArray received = tcp_socket->readAll();
    unprocessed += received;
    while(unprocessed.size()) {
        /* TODO: actually process event */
        unprocessed.remove(0, 1);
    }
}

void NetworkReceiver::disconnected() {
    emit no_more_data();
    quit();
}
