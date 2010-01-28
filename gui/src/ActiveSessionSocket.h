#ifndef AESALON_GUI_ACTIVE_SESSION_SOCKET_H
#define AESALON_GUI_ACTIVE_SESSION_SOCKET_H

#include <QObject>
#include <QTcpSocket>

namespace Aesalon {
namespace GUI {

class ActiveSessionSocket : public QObject { Q_OBJECT
private:
    QTcpSocket *socket;
    QString host;
    int port;
public:
    ActiveSessionSocket(QString host, int port);
    virtual ~ActiveSessionSocket();
public slots:
    void handle_data();
    void reemit_connected() { emit connected(); }
    void reemit_disconnected() { emit disconnected(); }
    void error_caught(QAbstractSocket::SocketError error);
    void try_connecting();
signals:
    void connected();
    void disconnected();
};

} // namespace GUI
} // namespace Aesalon

#endif
