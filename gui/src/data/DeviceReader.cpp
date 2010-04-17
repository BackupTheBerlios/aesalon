#include "DeviceReader.h"
#include "DeviceReader.moc"

DeviceReader::DeviceReader(QIODevice *device) : QThread(), device(device) {
    
}

DeviceReader::~DeviceReader() {

}

void DeviceReader::run() {
    const int max_data = 4096;
    connect(device, SIGNAL(readyRead()), SLOT(data_received()), Qt::QueuedConnection);
    while(device->isOpen()) {
        qDebug("device->isOpen() loop . . . current thread is %p", QThread::currentThread());
        QByteArray data = device->read(max_data);
        if(data.size() > 0) emit data_ready(data);
        if(data.size() != max_data) msleep(100);
    }
}

void DeviceReader::data_received() {
    emit data_ready(device->readAll());
}
