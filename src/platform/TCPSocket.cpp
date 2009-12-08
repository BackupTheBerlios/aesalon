#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <errno.h>

#include "TCPSocket.h"
#include "PlatformException.h"
#include "misc/StreamAsString.h"

namespace Aesalon {
namespace Platform {

TCPSocket::TCPSocket(std::string host, int port) {
    /* Open an internet socket, TCP. */
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd == -1) throw PlatformException("Couldn't create socket: ");
    
    struct sockaddr_in address;
    
    address.sin_family = AF_INET;
    address.sin_port = htons((uint16_t)port);
    std::memset(address.sin_zero, 0, sizeof(address.sin_zero));

    struct hostent *he;
    
    he = gethostbyname(host.c_str());
    if(he == NULL) throw PlatformException(Misc::StreamAsString() << "Couldn't lookup host: " << strerror(h_errno));
    address.sin_addr = *(struct in_addr *)he;
    
    if(connect(socket_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        throw PlatformException("Couldn't connect to host: ");
    }
    
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
