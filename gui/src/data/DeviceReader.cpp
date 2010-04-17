#include "DeviceReader.h"
#include "DeviceReader.moc"

DeviceReader::DeviceReader(QIODevice *device) : QThread(), device(device) {
    connect(device, SIGNAL(readyRead()), this, SLOT(data_received()), Qt::QueuedConnection);
}

DeviceReader::~DeviceReader() {

}

void DeviceReader::run() {
    const int max_data = 16384;
    while(device->isOpen()) {
        QByteArray data = device->read(max_data);
        if(data.size() > 0) emit data_ready(data);
        if(data.size() != max_data) msleep(50);
    }
}

void DeviceReader::data_received() {
    emit data_ready(device->readAll());
}
