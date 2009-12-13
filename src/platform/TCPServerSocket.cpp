#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <errno.h>

#include "TCPServerSocket.h"
#include "PlatformException.h"
#include "MemoryEvent.h"
#include "misc/String.h"

namespace Aesalon {
namespace Platform {

TCPServerSocket::TCPServerSocket(int port) : port(port) {
    struct addrinfo hints, *result, *rp;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
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
    
    for(rp = result; rp != NULL; rp = rp->ai_next) {
        socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(socket_fd == -1) continue;
        
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
    
    while((s_fd = accept(socket_fd, NULL, 0))) {
        socket_list.push_back(new TCPSocket(s_fd));
    }
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
        send_data(data->peek_event().to<MemoryEvent>()->serialize());
        data->pop_event();
    }
}

} // namespace Platform
} // namespace Aesalon
