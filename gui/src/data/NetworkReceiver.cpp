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

#include <QTime>

#include "NetworkReceiver.h"
#include "NetworkReceiver.moc"
#include "storage/AllocEvent.h"
#include "storage/FreeEvent.h"
#include "session/DataThread.h"

NetworkReceiver::NetworkReceiver(DataThread *data_thread, QString host, quint16 port) : DataReceiver(data_thread), host(host), port(port), start_time(0) {
    tcp_socket = new QTcpSocket(this);
    /*connect(tcp_socket, SIGNAL(readyRead()), this, SLOT(data_received()));*/
    /*recv_timer = new QTimer();
    connect(recv_timer, SIGNAL(timeout()), this, SLOT(process_queue()));
    recv_timer->start(100);*/
    /* NOTE: may want to do something with the disconnected signal . . . */
    
    device_reader = new DeviceReader(tcp_socket);
    connect(device_reader, SIGNAL(data_ready(QByteArray)), SLOT(data_received(QByteArray)), Qt::QueuedConnection);
    
    device_reader->start(QThread::HighestPriority);
    
    tcp_socket->connectToHost(host, port);
}

NetworkReceiver::~NetworkReceiver() {
    tcp_socket->close();
    device_reader->wait();
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

quint64 NetworkReceiver::pop_word(int bytes) {
    quint64 value = 0;
    if(unprocessed.size() < bytes) return 0;
    for(int i = 0; i < bytes; i ++) {
        value |= quint64(quint8(unprocessed.at(i) & 0xff)) << (i * 8);
    }
    unprocessed.remove(0, bytes);
    return value;
}

void NetworkReceiver::prepend_word(quint64 data, int bytes) {
    for(int i = bytes-1; i >= 0; i --) {
        unprocessed.prepend(char((data << (i * 8)) & 0xff));
    }
}

Backtrace NetworkReceiver::assemble_backtrace() {
    if(unprocessed.size() < 2) return Backtrace(NULL, 0);
    /*quint16 count = pop_word(2);*/
    quint16 count = 0;
    count = unprocessed.at(0);
    count |= quint16(unprocessed.at(1)) << 8;
    if(unprocessed.size() < count * 4) {
        return Backtrace(NULL, 0);
    }
    unprocessed.remove(0, 2);
    Scope *scopes = new Scope[count];
    
    for(quint16 i = 0; i < count; i ++) {
        scopes[i] = get_data_thread()->get_scope_mapper()->get_scope(pop_word(4));
    }
    
    return Backtrace(scopes, count);
}

void NetworkReceiver::data_received(QByteArray data) {
    unprocessed += data;
    process_queue();
}

void NetworkReceiver::process_queue() {
    while(unprocessed.size()) {
        quint8 header = unprocessed.at(0);
        unprocessed.remove(0, 1);
        int word_size = (header & 0x80)?8:4;
        /* If the first bit is set, and the second is not, then it's a block event . . . */
        if((header & 0x03) == 1) {
            quint8 block_type = (header & 0x0c) >> 2;
            /* Block event types:
                0: allocation event:
                    + scope
                    + address
                    + size
                1: resize event:
                    + scope
                    + address
                    + size
                    + new_address
                2: free event:
                    + scope
                    + address
            */
            quint8 block_type_sizes[] = {2, 3, 1};
            /* Check the size of the waiting data.
                8 extra bytes for the timestamp. */
            if(unprocessed.size() < ((block_type_sizes[block_type] * word_size)+8)) {
                unprocessed.prepend(header);
                break;
            }
            quint64 raw_timestamp = pop_quint64();
            Timestamp timestamp = Timestamp(start_time.ns_until(Timestamp(raw_timestamp)));
            quint64 words[3];
            for(int i = 0; i < block_type_sizes[block_type]; i ++) words[i] = pop_word(word_size);
            
            Backtrace backtrace = assemble_backtrace();
            if(backtrace.get_scope_list() == NULL) {
                for(int i = 0; i < block_type_sizes[block_type]; i ++) prepend_word(words[i], word_size);
                break;
            }
            
            if(block_type == 0) {
                event_received(new AllocEvent(timestamp, words[0], words[1], backtrace));
            }
            else if(block_type == 1) {
                /* From the man page for realloc: "If ptr is NULL, then the call is equivalent to malloc(size),
                    for all values of size; if size is equal to zero, and ptr is not NULL, then the call is equivalent to free(ptr)."
                    Ergo, don't emit free/alloc events for such cases. */
                if(words[0] != 0) event_received(new FreeEvent(timestamp, words[0], backtrace));
                if(words[2] != 0) event_received(new AllocEvent(timestamp, words[1], words[2], backtrace));
            }
            else if(block_type == 2) {
                event_received(new FreeEvent(timestamp, words[0], backtrace));
            }
        }
        else if((header & 0x03) == 2) {
            if(unprocessed.size() < 8) {
                unprocessed.prepend(header);
                break;
            }
            quint64 raw_timestamp = pop_quint64();
            Timestamp *timestamp = new Timestamp(raw_timestamp);
            /* one indicates finished, zero started. */
            if((header & 0x04) == 0) {
                start_time = *timestamp;
                emit started(timestamp);
            }
            else emit finished(timestamp);
        }
        else if((header & 0x03) == 3) {
            /* there should be at least 9 bytes left: 8 for timestamp, 1 for name size. */
            if(unprocessed.size() < 9) {
                unprocessed.prepend(header);
                break;
            }
            /* ignore the timestamp . . . */
            quint8 size = unprocessed.at(8);
            if(unprocessed.size() < size + 9) {
                unprocessed.prepend(header);
                break;
            }
            unprocessed.remove(0, 9);
            QByteArray scope_name;
            scope_name = unprocessed.left(size);
            unprocessed.remove(0, size);
            get_data_thread()->get_scope_mapper()->add_scope(Scope(scope_name));
        }
        else {
            qCritical(
                "Invalid event type encountered (%i). Since said event is an unknown size, "
                "all further processing has been halted.", (header & 0x03));
            unprocessed.prepend(header);
            break;
        }
    }
}

void NetworkReceiver::connected() {
    /*emit started();*/
}

void NetworkReceiver::disconnected() {
    device_reader->quit();
    emit finished(new Timestamp());
}
