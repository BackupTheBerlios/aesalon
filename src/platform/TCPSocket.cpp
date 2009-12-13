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
    std::cout << "TCPSocket::TCPSocket(): beginning initialization . . ." << std::endl;
    std::cout << "TCPSocket::TCPSocket(): attempting to connect to " << host << ":" << port << std::endl;
    struct addrinfo hints, *result;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = hints.ai_protocol = 0;
    std::cout << "TCPSocket::TCPSocket(): resolving address . . ." << std::endl;
    int ret = getaddrinfo(host.c_str(), Misc::String::from<int>(port).c_str(), &hints, &result);
    if(ret != 0) {
        throw PlatformException(Misc::StreamAsString() << "Couldn't resolve hostname: " << gai_strerror(ret), false);
    }
    
    if(result->ai_next != NULL) {
        std::cout << "Warning: Hostname \"" << host << "\" resolves to multiple IPs. Using the first." << std::endl;
    }
    
    
    std::cout << "TCPSocket::TCPSocket(): creating socket . . ." << std::endl;
    socket_fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if(socket_fd == -1) throw PlatformException("Couldn't create socket: ");
    
    std::cout << "TCPSocket::TCPSocket(): connecting socket . . ." << std::endl;
    std::cout << "TCPSocket::TCPSocket(): socket_fd is: " << socket_fd << std::endl;
    std::cout << "TCPSocket::TCPSocket(): result->ai_addr is: " << result->ai_addr << std::endl;
    if(connect(socket_fd, result->ai_addr, result->ai_addrlen) == -1) throw PlatformException("Couldn't connect to host: ");
    
    std::cout << "Connected successfully!" << std::endl;
    
    /* Socket is now connected. */
    valid = true;
}
TCPSocket::~TCPSocket() {
    close(socket_fd);
}

void TCPSocket::send_data(std::string data) {
    std::string raw_data;
    
    raw_data = Misc::StreamAsString() << htons(data.length()) << data;
    
    int sent = write(socket_fd, raw_data.c_str(), raw_data.length());
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

} // namespace Platform
} // namespace Aesalon
