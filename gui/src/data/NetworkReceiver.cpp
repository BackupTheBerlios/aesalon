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
    if(unprocessed.size() < 8) {
        return 0;
    }
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

void NetworkReceiver::prepend_quint64(quint64 data) {
    unprocessed.prepend(char((data << 56) & 0xff));
    unprocessed.prepend(char((data << 48) & 0xff));
    unprocessed.prepend(char((data << 40) & 0xff));
    unprocessed.prepend(char((data << 32) & 0xff));
    unprocessed.prepend(char((data << 24) & 0xff));
    unprocessed.prepend(char((data << 16) & 0xff));
    unprocessed.prepend(char((data << 8) & 0xff));
    unprocessed.prepend(char(data & 0xff));
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
            quint8 block_type_sizes[] = {16, 24, 8};
            if(unprocessed.size() < (block_type_sizes[block_type]+8)) {
                unprocessed.prepend(type_byte);
                break;
            }
            quint64 raw_timestamp = pop_quint64();
            Timestamp timestamp = Timestamp(raw_timestamp);
            quint64 address = pop_quint64();
            if(block_type == 0) {
                quint64 size = pop_quint64();
                emit event_received(new AllocEvent(timestamp, address, size));
            }
            else if(block_type == 1) {
                quint64 new_address = pop_quint64();
                quint64 new_size = pop_quint64();
                /* From the man page for realloc: "If ptr is NULL, then the call is equivalent to malloc(size),
                    for all values of size; if size is equal to zero, and ptr is not NULL, then the call is equivalent to free(ptr).
                    Ergo, don't emit free/alloc events for such cases. */
                if(address != 0) emit event_received(new FreeEvent(timestamp, address));
                if(new_size != 0) emit event_received(new AllocEvent(timestamp, new_address, new_size));
            }
            else if(block_type == 2) {
                emit event_received(new FreeEvent(timestamp, address));
            }
        }
    }
}

void NetworkReceiver::connected() {
    emit started();
}

void NetworkReceiver::disconnected() {
    emit finished();
}
