#ifndef AESALON_PLATFORM_TCPSERVER_SOCKET_H
#define AESALON_PLATFORM_TCPSERVER_SOCKET_H

#include <vector>

#include "TCPSocket.h"
#include "EventQueue.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Platform {

class TCPServerSocket {
public:
    typedef std::vector<Misc::SmartPointer<TCPSocket> > socket_list_t;
private:
    int socket_fd;
    int port;
    socket_list_t socket_list;
public:
    TCPServerSocket(int port);
    virtual ~TCPServerSocket();
    
    void accept_connections();
    void remove_invalid_sockets();
    
    void send_data(std::string data);
    void send_data(Misc::SmartPointer<EventQueue> data);
    
    int get_port() const;
};

} // namespace Platform
} // namespace Aesalon

#endif
