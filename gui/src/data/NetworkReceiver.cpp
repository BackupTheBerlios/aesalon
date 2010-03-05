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
        interrupted = true;
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
            quint64 address = pop_quint64();
            if(interrupted) {
                unprocessed.prepend(type_byte);
                break;
            }
            if(block_type == 0) {
                quint64 size = pop_quint64();
                if(interrupted) {
                    prepend_quint64(address);
                    break;
                }
                emit event_received(new AllocEvent(address, size));
            }
            else if(block_type == 1) {
                quint64 new_address = pop_quint64();
                if(interrupted) {
                    prepend_quint64(address);
                    break;
                }
                quint64 new_size = pop_quint64();
                if(interrupted) {
                    prepend_quint64(address);
                    prepend_quint64(new_address);
                    break;
                }
                qDebug("Realloc event received: old address 0x%llx, new address 0x%llx, new size 0x%llx", address, new_address, new_size);
                emit event_received(new FreeEvent(address));
                emit event_received(new AllocEvent(new_address, new_size));
            }
            else if(block_type == 2) {
                emit event_received(new FreeEvent(address));
            }
        }
    }
    interrupted = false;
}

void NetworkReceiver::connected() {
    emit started();
}

void NetworkReceiver::disconnected() {
    emit finished();
}
