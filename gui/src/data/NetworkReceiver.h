#ifndef AESALON_GUI_NETWORK_RECEIVER_H
#define AESALON_GUI_NETWORK_RECEIVER_H

#include <QTcpSocket>

#include "DataReceiver.h"

class NetworkReceiver : public DataReceiver { Q_OBJECT
private:
    QString host;
    quint16 port;
    QTcpSocket *tcp_socket;
    QByteArray unprocessed;
public:
    NetworkReceiver(DataThread *data_thread, QString host, quint16 port);
    virtual ~NetworkReceiver();
private:
    quint64 pop_quint64();
private slots:
    void data_received();
    void connected();
    void disconnected();
};

#endif
