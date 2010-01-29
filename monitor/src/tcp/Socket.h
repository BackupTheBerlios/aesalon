#ifndef AESALON_MONITOR_TCPSOCKET_H
#define AESALON_MONITOR_TCPSOCKET_H

#include <string>

#include "Types.h"

namespace TCP {

class Socket {
private:
    int socket_fd;
    bool valid;
public:
    Socket(std::string host, int port);
    Socket(int socket_fd) : socket_fd(socket_fd), valid(true) {}
    virtual ~Socket();
    
    void send_data(Block *data);
    
    void disconnect();
    
    bool is_valid() const { return valid; }
};

} // namespace TCP

#endif
