#ifndef AESALON_MONITOR_TCPSERVER_SOCKET_H
#define AESALON_MONITOR_TCPSERVER_SOCKET_H

#include <vector>

#include "TCPSocket.h"
#include "misc/EventQueue.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Monitor {

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
    void wait_for_connection();
    void remove_invalid_sockets();
    void disconnect_all();
    
    void send_data(std::string data);
    void send_data(Misc::SmartPointer<Misc::EventQueue> data);
    
    int get_port() const;
};

} // namespace Monitor
} // namespace Aesalon

#endif
