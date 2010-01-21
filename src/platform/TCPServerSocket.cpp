#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <errno.h>

#include "TCPServerSocket.h"
#include "PlatformException.h"
#include "misc/String.h"

namespace Aesalon {
namespace Platform {

TCPServerSocket::TCPServerSocket(int port) : port(port) {
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
        throw PlatformException(Misc::StreamAsString() << "Couldn't resolve hostname: " << gai_strerror(ret), false);
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
    if(rp == NULL) throw PlatformException("Couldn't open port for listening: ");
    freeaddrinfo(result);
    
    if(listen(socket_fd, 8) == -1) throw PlatformException("Couldn't listen on socket: ");
}

TCPServerSocket::~TCPServerSocket() {
    
    close(socket_fd);
}

int TCPServerSocket::get_port() const {
    if(port != 0) return port;
    
    /* TODO: find the randomly-allocated port . . . */
    return 0;
}

void TCPServerSocket::accept_connections() {
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
            s_fd = accept(socket_fd, NULL, 0);
            socket_list.push_back(new TCPSocket(s_fd));
        }
        read_set = listen_set;
    }
}

void TCPServerSocket::wait_for_connection() {
    int s_fd = accept(socket_fd, NULL, 0);
    socket_list.push_back(new TCPSocket(s_fd));
}

void TCPServerSocket::remove_invalid_sockets() {
    socket_list_t::iterator i = socket_list.begin();
    
    /* NOTE: this is quite inefficient with large amounts of sockets . . . */
    for(; i != socket_list.end(); i ++) {
        if(!(*i)->is_valid()) {
            (*i) = 0;
            socket_list.erase(i);
            i = socket_list.begin();
        }
    }
}

void TCPServerSocket::send_data(std::string data) {
    socket_list_t::iterator i = socket_list.begin();
    
    for(; i != socket_list.end(); i ++) {
        (*i)->send_data(data);
    }
}

void TCPServerSocket::send_data(Misc::SmartPointer<EventQueue> data) {
    while(data->peek_event()) {
        std::string data_string = data->peek_event()->serialize();
        std::cout << "TCPServerSocket::send_data(): sending \"" << data_string << "\"\n";
        send_data(data_string);
        data->pop_event();
    }
}

void TCPServerSocket::disconnect_all() {
    socket_list_t::iterator i = socket_list.begin();
    for(; i != socket_list.end(); i ++) {
        if((*i)->is_valid()) (*i)->disconnect();
    }
}

} // namespace Platform
} // namespace Aesalon
