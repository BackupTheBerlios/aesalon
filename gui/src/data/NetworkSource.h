#ifndef AESALON_GUI_NETWORK_SOURCE_H
#define AESALON_GUI_NETWORK_SOURCE_H

#include "DataSource.h"
#include "NetworkReceiver.h"

class NetworkSource : public DataSource {
private:
    QString host;
    quint16 port;
public:
    NetworkSource(QString name, QString host, quint16 port) : DataSource(name), host(host), port(port) {}
    virtual ~NetworkSource() {}
    
    DataReceiver *new_receiver() const { return new NetworkReceiver(host, port); }
};

#endif
