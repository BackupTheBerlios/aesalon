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
    
    @file tcp/ServerSocket.h
*/

#ifndef AESALON_MONITOR_TCPSERVER_SOCKET_H
#define AESALON_MONITOR_TCPSERVER_SOCKET_H

#include <vector>

#include "Socket.h"
#include "event/Queue.h"

namespace TCP {

class ServerSocket {
public:
    typedef std::vector<Socket *> socket_list_t;
private:
    int socket_fd;
    int port;
    socket_list_t socket_list;
public:
    ServerSocket(int port);
    virtual ~ServerSocket();
    
    void accept_connections();
    void wait_for_connection();
    void remove_invalid_sockets();
    void disconnect_all();
    
    void send_data(Block *data);
    void send_data(Event::Queue *data);
    
    int get_port() const;
};

} // namespace TCP

#endif
