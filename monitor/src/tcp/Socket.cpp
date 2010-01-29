#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <errno.h>

#include "Socket.h"
#include "misc/StreamAsString.h"
#include "misc/String.h"
#include "exception/TCPException.h"


namespace TCP {

Socket::Socket(std::string host, int port) {
    struct addrinfo hints, *result;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = hints.ai_protocol = 0;
    int ret = getaddrinfo(host.c_str(), Misc::String::from<int>(port).c_str(), &hints, &result);
    if(ret != 0) {
        throw Exception::TCPException(Misc::StreamAsString() << "Couldn't resolve hostname: " << gai_strerror(ret));
    }
    
    if(result->ai_next != NULL) {
        std::cout << "Warning: Hostname \"" << host << "\" resolves to multiple IPs. Using the first." << std::endl;
    }
    
    
    socket_fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if(socket_fd == -1) throw Exception::TCPException(Misc::StreamAsString() << "Couldn't create socket: " << strerror(errno));
    
    if(connect(socket_fd, result->ai_addr, result->ai_addrlen) == -1) {
        close(socket_fd);
        throw Exception::TCPException(Misc::StreamAsString() << "Couldn't connect to host: " << strerror(errno));
    }
    
    /* Socket is now connected. */
    valid = true;
}

Socket::~Socket() {
    if(socket_fd) close(socket_fd);
}

void Socket::send_data(Block *data) {
    int sent = write(socket_fd, data->get_data(), data->get_size());
    if(sent == -1) valid = false;
}

/*std::string Socket::get_data() {
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
}*/

void Socket::disconnect() {
    if(socket_fd) {
        close(socket_fd);
        socket_fd = 0;
    }
}

} // namespace TCP
