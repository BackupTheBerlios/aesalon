#ifndef AESALON_GUI_NETWORK_RECEIVER_H
#define AESALON_GUI_NETWORK_RECEIVER_H

#include <QTcpSocket>

#include "DataReceiver.h"

class NetworkReceiver : public DataReceiver { Q_OBJECT
private:
    QTcpSocket *tcp_socket;
    QByteArray unprocessed;
public:
    NetworkReceiver(QString host, quint16 port);
    virtual ~NetworkReceiver();
protected:
    virtual void run();
private slots:
    void data_received();
    void disconnected();
};

#endif
