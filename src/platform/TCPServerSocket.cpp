#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

#include "TCPServerSocket.h"
#include "PlatformException.h"

namespace Aesalon {
namespace Platform {

TCPServerSocket::TCPServerSocket(int port) : port(port) {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd == -1) throw PlatformException("Unable to create socket: ");
    
    struct sockaddr_in address;
    
    address.sin_port = htons(port);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(address.sin_zero, 0, sizeof(address.sin_zero));
    
    int yes = 1;
    if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        throw PlatformException("Couldn't set socket option SO_REUSEADDR: ");
    }
    
    bind(socket_fd, (struct sockaddr *)&address, sizeof(address));
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

} // namespace Platform
} // namespace Aesalon
