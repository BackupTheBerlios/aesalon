#ifndef AESALON_GUI_ACTIVE_SESSION_SOCKET_H
#define AESALON_GUI_ACTIVE_SESSION_SOCKET_H

#include <QObject>
#include <QTcpSocket>

#include "platform/Memory.h"

namespace Aesalon {
namespace GUI {

class ActiveSessionSocket : public QObject { Q_OBJECT
private:
    Platform::Memory *memory;
    QTcpSocket *socket;
public:
    ActiveSessionSocket(QString host, int port, Platform::Memory *memory);
    virtual ~ActiveSessionSocket();
public slots:
    void handle_data();
    void reemit_connected() { emit connected(); }
signals:
    void connected();
};

} // namespace GUI
} // namespace Aesalon

#endif
