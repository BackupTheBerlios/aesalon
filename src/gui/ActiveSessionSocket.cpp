#include "ActiveSessionSocket.h"
#include "ActiveSessionSocket.moc"

namespace Aesalon {
namespace GUI {

ActiveSessionSocket::ActiveSessionSocket(QString host, int port, Platform::Memory *memory,
    QMutex *memory_mutex) : QThread(), memory(memory), memory_mutex(memory_mutex) {
    
    socket = new QTcpSocket();
    
    socket->connectToHost(host, port);
}

ActiveSessionSocket::~ActiveSessionSocket() {
}

void ActiveSessionSocket::run() {
    if(!socket->waitForConnected(60000)) {
        /* Couldn't connect within 60 seconds . . . */
        exit(1);
    }
    QByteArray data_queue;
    while(socket->isValid()) {
        QByteArray temporary = socket->read(socket->bytesAvailable());
        data_queue += temporary;
        QString string = data_queue;
        Platform::Event *event = Platform::Event::deserialize(string.toStdString());
        data_queue.remove(0, string.length());
        memory_mutex->lock();
        while(event) {
            memory->handle_event(event);
            
            string = data_queue;
            event = Platform::Event::deserialize(string.toStdString());
            data_queue.remove(0, string.length());
        }
        memory_mutex->unlock();
        this->msleep(500);
    }
}

} // namespace GUI
} // namespace Aesalon
