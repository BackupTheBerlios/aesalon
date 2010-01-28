#ifndef AESALON_MONITOR_TCPSOCKET_H
#define AESALON_MONITOR_TCPSOCKET_H

#include <string>

namespace Aesalon {
namespace Monitor {

class TCPSocket {
private:
    int socket_fd;
    bool valid;
public:
    TCPSocket(std::string host, int port);
    TCPSocket(int socket_fd) : socket_fd(socket_fd), valid(true) {}
    virtual ~TCPSocket();
    
    void send_data(std::string data);
    std::string get_data();
    
    void disconnect();
    
    bool is_valid() const { return valid; }
};

} // namespace Monitor
} // namespace Aesalon

#endif
