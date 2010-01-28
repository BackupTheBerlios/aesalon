#ifndef AESALON_MONITOR_TCPSERVER_SOCKET_H
#define AESALON_MONITOR_TCPSERVER_SOCKET_H

#include <vector>

#include "Socket.h"
#include "event/Queue.h"

namespace TCP {

class ServerSocket {
public:
    typedef std::vector<Socket *> socket_list_t;
private:
    int socket_fd;
    int port;
    socket_list_t socket_list;
public:
    ServerSocket(int port);
    virtual ~ServerSocket();
    
    void accept_connections();
    void wait_for_connection();
    void remove_invalid_sockets();
    void disconnect_all();
    
    void send_data(std::string data);
    void send_data(Event::Queue *data);
    
    int get_port() const;
};

} // namespace TCP

#endif
