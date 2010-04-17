#ifndef AESALON_GUI_DATA_DEVICE_READER_H
#define AESALON_GUI_DATA_DEVICE_READER_H

#include <QThread>
#include <QIODevice>

class DeviceReader : public QThread { Q_OBJECT
private:
    QIODevice *device;
public:
    DeviceReader(QIODevice *device);
    virtual ~DeviceReader();
    
    virtual void run();
private slots:
    void data_received();
signals:
    void data_ready(QByteArray data);
};

#endif
