#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <errno.h>

#include "TCPSocket.h"
#include "PlatformException.h"
#include "misc/StreamAsString.h"
#include "misc/String.h"

namespace Aesalon {
namespace Platform {

TCPSocket::TCPSocket(std::string host, int port) {
    struct addrinfo hints, *result;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = hints.ai_protocol = 0;
    int ret = getaddrinfo(host.c_str(), Misc::String::from<int>(port).c_str(), &hints, &result);
    if(ret != 0) {
        throw PlatformException(Misc::StreamAsString() << "Couldn't resolve hostname: " << gai_strerror(ret), false);
    }
    
    if(result->ai_next != NULL) {
        std::cout << "Warning: Hostname \"" << host << "\" resolves to multiple IPs. Using the first." << std::endl;
    }
    
    
    socket_fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if(socket_fd == -1) throw PlatformException("Couldn't create socket: ");
    
    if(connect(socket_fd, result->ai_addr, result->ai_addrlen) == -1) {
        close(socket_fd);
        throw PlatformException("Couldn't connect to host: ");
    }
    
    /* Socket is now connected. */
    valid = true;
}
TCPSocket::~TCPSocket() {
    if(socket_fd) close(socket_fd);
}

void TCPSocket::send_data(std::string data) {
    int sent = write(socket_fd, data.c_str(), data.length());
    if(sent == -1) valid = false;
}
std::string TCPSocket::get_data() {
    if(!is_valid()) return "";
    uint16_t size;
    read(socket_fd, &size, 2);
    size = ntohs(size);
    
    std::string data;
    
    while(size) {
        char c;
        int ret = read(socket_fd, &c, 1);
        if(ret == -1) {
            if(errno == EAGAIN) continue;
            else {
                valid = false;
                break;
            }
        }
        data += c;
        size --;
    }
    
    return data;
}

void TCPSocket::disconnect() {
    if(socket_fd) {
        close(socket_fd);
        socket_fd = 0;
    }
}

} // namespace Platform
} // namespace Aesalon
