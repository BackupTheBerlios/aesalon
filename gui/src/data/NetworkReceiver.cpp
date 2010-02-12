#include "NetworkReceiver.h"
#include "NetworkReceiver.moc"
#include "storage/AllocEvent.h"
#include "storage/FreeEvent.h"

NetworkReceiver::NetworkReceiver(QObject* parent, QString host, quint16 port) : DataReceiver(parent), host(host), port(port) {
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

quint64 NetworkReceiver::pop_quint64() {
    quint64 ret = 0;
    ret |= unprocessed.at(0);
    ret |= quint64(quint8(unprocessed.at(1))) << 8;
    ret |= quint64(quint8(unprocessed.at(2))) << 16;
    ret |= quint64(quint8(unprocessed.at(3))) << 24;
    ret |= quint64(quint8(unprocessed.at(4))) << 32;
    ret |= quint64(quint8(unprocessed.at(5))) << 40;
    ret |= quint64(quint8(unprocessed.at(6))) << 48;
    ret |= quint64(quint8(unprocessed.at(7))) << 56;
    unprocessed.remove(0, 8);
    return ret;
}

void NetworkReceiver::data_received() {
    QByteArray received = tcp_socket->readAll();
    unprocessed += received;
    while(unprocessed.size()) {
        quint8 type_byte = unprocessed.at(0);
        unprocessed.remove(0, 1);
        /* If the first bit is set, then it's a block event . . . */
        if(type_byte & 0x01) {
            quint64 address = pop_quint64();
            if((type_byte & 0x06) == 0) {
                emit event_received(new AllocEvent(address, pop_quint64()));
            }
            else if((type_byte & 0x06) == 1) {
                emit event_received(new FreeEvent(address));
                emit event_received(new AllocEvent(pop_quint64(), pop_quint64()));
            }
            else if((type_byte & 0x06) == 2) {
                emit event_received(new FreeEvent(address));
            }
        }
    }
}

void NetworkReceiver::disconnected() {
    emit no_more_data();
    quit();
}
