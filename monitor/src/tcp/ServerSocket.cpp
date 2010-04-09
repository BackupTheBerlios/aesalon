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
    
    @file tcp/ServerSocket.cpp
*/

#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <errno.h>

#include "ServerSocket.h"
#include "exception/TCPException.h"
#include "misc/String.h"
#include "Initializer.h"

namespace TCP {

ServerSocket::ServerSocket(int port) : port(port) {
    struct addrinfo hints, *result, *rp;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_family = AF_INET; /* NOTE: this is to prevent listening on TCP6 sockets for now. */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    
    int ret = getaddrinfo(NULL, Misc::String::from<int>(port).c_str(), &hints, &result);
    if(ret != 0) {
        throw Exception::TCPException(Misc::StreamAsString() << "Couldn't resolve hostname: " << gai_strerror(ret));
    }
    
    int yes = 1;
    
    for(rp = result; rp != NULL; rp = rp->ai_next) {
        socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(socket_fd == -1) continue;
        
        if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
            continue;
        
        if(bind(socket_fd, rp->ai_addr, rp->ai_addrlen) == 0) break;
        
        int e = errno;
        close(socket_fd);
        errno = e;
    }
    if(rp == NULL) throw Exception::TCPException(Misc::StreamAsString() << "Couldn't open port for listening: " << strerror(errno));
    freeaddrinfo(result);
    
    if(listen(socket_fd, 8) == -1) throw Exception::TCPException(Misc::StreamAsString() << "Couldn't listen on socket: " << strerror(errno));
}

ServerSocket::~ServerSocket() {
    
    close(socket_fd);
}

int ServerSocket::get_port() const {
    if(port != 0) return port;
    
    /* TODO: find the randomly-allocated port . . . */
    return 0;
}

void ServerSocket::accept_connections() {
    int s_fd;
    
    fd_set listen_set, read_set;
    struct timeval tv;
    FD_ZERO(&listen_set);
    FD_ZERO(&read_set);
    FD_SET(socket_fd, &listen_set);
    
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    read_set = listen_set;
    
    while(select(socket_fd+1, &read_set, NULL, NULL, &tv)) {
        if(FD_ISSET(socket_fd, &read_set)) {
            std::cout << "Socket wants a connection!" << std::endl;
            s_fd = accept(socket_fd, NULL, 0);
            socket_list.push_back(new Socket(s_fd));
        }
        read_set = listen_set;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
    }
}

void ServerSocket::wait_for_connection() {
    int s_fd = accept(socket_fd, NULL, 0);
    socket_list.push_back(new Socket(s_fd));
}

void ServerSocket::remove_invalid_sockets() {
    socket_list_t::iterator i = socket_list.begin();
    
    /* NOTE: this is quite inefficient with large amounts of sockets . . . */
    for(; i != socket_list.end(); i ++) {
        if(!(*i)->is_valid()) {
            delete *i;
            socket_list.erase(i);
            i = socket_list.begin();
        }
    }
}

void ServerSocket::send_data(Block *data) {
    socket_list_t::iterator i = socket_list.begin();
    
    for(; i != socket_list.end(); i ++) {
        (*i)->send_data(data);
    }
}

void ServerSocket::send_data(Event::Queue *data) {
    static int bits = 0;
    if(bits == 0) {
        bits = Initializer::get_instance()->get_analyzer_interface()->get_file()->get_attribute("platform")->get_attribute("bits")->get_value();
    }
    data->lock_mutex();
    while(data->peek_event()) {
        Block *raw_data = data->peek_event()->serialize(bits);
        send_data(raw_data);
        data->pop_event();
        delete raw_data;
    }
    data->unlock_mutex();
}

void ServerSocket::disconnect_all() {
    socket_list_t::iterator i = socket_list.begin();
    for(; i != socket_list.end(); i ++) {
        if((*i)->is_valid()) (*i)->disconnect();
    }
}

} // namespace TCP