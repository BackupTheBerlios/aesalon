
#include "NetworkReceiver.h"
#include "NetworkReceiver.moc"
#include "storage/AllocEvent.h"
#include "storage/FreeEvent.h"

NetworkReceiver::NetworkReceiver(DataThread *data_thread, QString host, quint16 port) : DataReceiver(data_thread), host(host), port(port) {
    tcp_socket = new QTcpSocket(this);
    tcp_socket->connectToHost(host, port);
    connect(tcp_socket, SIGNAL(readyRead()), this, SLOT(data_received()));
    connect(tcp_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(tcp_socket, SIGNAL(connected()), SLOT(connected()));
}

NetworkReceiver::~NetworkReceiver() {
}

quint64 NetworkReceiver::pop_quint64() {
    quint64 ret = 0;
    ret |= unprocessed.at(0) & 0xff;
    ret |= quint64(quint8(unprocessed.at(1)) & 0xff) << 8;
    ret |= quint64(quint8(unprocessed.at(2)) & 0xff) << 16;
    ret |= quint64(quint8(unprocessed.at(3)) & 0xff) << 24;
    ret |= quint64(quint8(unprocessed.at(4)) & 0xff) << 32;
    ret |= quint64(quint8(unprocessed.at(5)) & 0xff) << 40;
    ret |= quint64(quint8(unprocessed.at(6)) & 0xff) << 48;
    ret |= quint64(quint8(unprocessed.at(7)) & 0xff) << 56;
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
            quint8 block_type = (type_byte & 0x06) >> 1;
            quint64 address = pop_quint64();
            if(block_type == 0) {
                emit event_received(new AllocEvent(address, pop_quint64()));
            }
            else if(block_type == 1) {
                emit event_received(new FreeEvent(address));
                emit event_received(new AllocEvent(pop_quint64(), pop_quint64()));
            }
            else if(block_type == 2) {
                qDebug("Constructing new FreeEvent, address is 0x%x . . .", address);
                emit event_received(new FreeEvent(address));
            }
        }
    }
}

void NetworkReceiver::connected() {
    qDebug("NetworkReceiver::connected() . . .");
    emit started();
}

void NetworkReceiver::disconnected() {
    emit finished();
}
