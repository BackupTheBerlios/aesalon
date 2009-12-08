#ifndef AESALON_PLATFORM_TCPSOCKET_H
#define AESALON_PLATFORM_TCPSOCKET_H

#include <string>

namespace Aesalon {
namespace Platform {

class TCPSocket {
private:
    int socket_fd;
public:
    TCPSocket(std::string host, int port);
    virtual ~TCPSocket();
    
    void send_data(std::string data);
    std::string get_data();
};

} // namespace Platform
} // namespace Aesalon

#endif
