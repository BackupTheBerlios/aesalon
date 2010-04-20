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
    
    @file data/NetworkReceiver.h
*/

#ifndef AESALON_GUI_NETWORK_RECEIVER_H
#define AESALON_GUI_NETWORK_RECEIVER_H

#include <QTimer>
#include <QTcpSocket>

#include "DataReceiver.h"

class NetworkReceiver : public DataReceiver { Q_OBJECT
private:
    QString host;
    quint16 port;
    QTcpSocket *tcp_socket;
    QTimer *recv_timer;
    QByteArray unprocessed;
    Timestamp start_time;
    
    int position;
    int word_size;
public:
    NetworkReceiver(DataThread *data_thread, QString host, quint16 port);
    virtual ~NetworkReceiver();
private:
    quint64 pop_quint64();
    quint64 pop_word();
    inline int remaining();
private slots:
    void data_received();
    void process_queue();
    void connected();
    void disconnected();
};

#endif
