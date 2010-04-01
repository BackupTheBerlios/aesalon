/** Aesalon, a project to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009-2010 strange <kawk256@gmail.com>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    @file data/NetworkReceiver.cpp
*/

#include "NetworkReceiver.h"
#include "NetworkReceiver.moc"
#include "storage/AllocEvent.h"
#include "storage/FreeEvent.h"

NetworkReceiver::NetworkReceiver(DataThread *data_thread, QString host, quint16 port) : DataReceiver(data_thread), host(host), port(port), start_time(0) {
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
        /* If the first bit is set, and the second is not, then it's a block event . . . */
        if((type_byte & 0x03) == 1) {
            quint8 block_type = (type_byte & 0x0c) >> 2;
            quint8 block_type_sizes[] = {16, 24, 8};
            /* Check the size of the waiting data. At least 16 more bytes are required (8 for timestamp, 8 for scope). */
            if(unprocessed.size() < (block_type_sizes[block_type]+16)) {
                unprocessed.prepend(type_byte);
                break;
            }
            quint64 raw_timestamp = pop_quint64();
            Timestamp timestamp = Timestamp(start_time.ns_until(Timestamp(raw_timestamp)));
            quint64 scope_address = pop_quint64();
            quint64 address = pop_quint64();
            if(block_type == 0) {
                quint64 size = pop_quint64();
                event_received(new AllocEvent(timestamp, address, size, scope_address));
            }
            else if(block_type == 1) {
                quint64 new_address = pop_quint64();
                quint64 new_size = pop_quint64();
                /* From the man page for realloc: "If ptr is NULL, then the call is equivalent to malloc(size),
                    for all values of size; if size is equal to zero, and ptr is not NULL, then the call is equivalent to free(ptr)."
                    Ergo, don't emit free/alloc events for such cases. */
                if(address != 0) event_received(new FreeEvent(timestamp, address, scope_address));
                if(new_size != 0) event_received(new AllocEvent(timestamp, new_address, new_size, scope_address));
            }
            else if(block_type == 2) {
                event_received(new FreeEvent(timestamp, address, scope_address));
            }
        }
        else if((type_byte & 0x03) == 2) {
            if(unprocessed.size() < 8) {
                unprocessed.prepend(type_byte);
                break;
            }
            quint64 raw_timestamp = pop_quint64();
            Timestamp *timestamp = new Timestamp(raw_timestamp);
            /* one indicates finished, zero started. */
            if((type_byte & 0x04) == 0) {
                start_time = *timestamp;
                emit started(timestamp);
            }
            else emit finished(timestamp);
        }
    }
}

void NetworkReceiver::connected() {
    /*emit started();*/
}

void NetworkReceiver::disconnected() {
    emit finished(new Timestamp());
}
