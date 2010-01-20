#ifndef AESALON_GUI_ACTIVE_SESSION_SOCKET_H
#define AESALON_GUI_ACTIVE_SESSION_SOCKET_H

#include <QThread>
#include <QMutex>

#include <QTcpSocket>

#include "platform/Memory.h"

namespace Aesalon {
namespace GUI {

class ActiveSessionSocket : public QThread { Q_OBJECT
private:
    Platform::Memory *memory;
    QMutex *memory_mutex;
    QTcpSocket *socket;
public:
    ActiveSessionSocket(QString host, int port, Platform::Memory *memory, QMutex *memory_mutex);
    virtual ~ActiveSessionSocket();
    
    virtual void run();
};

} // namespace GUI
} // namespace Aesalon

#endif
