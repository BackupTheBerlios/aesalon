#ifndef AESALON_GUI_NETWORK_RECEIVER_H
#define AESALON_GUI_NETWORK_RECEIVER_H

#include "DataReceiver.h"

class NetworkReceiver : public DataReceiver { Q_OBJECT
public:
    NetworkReceiver(QString host, quint16 port);
    virtual ~NetworkReceiver();
protected:
    virtual void run();
};

#endif
