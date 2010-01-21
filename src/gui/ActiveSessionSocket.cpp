#include <iostream>
#include "ActiveSessionSocket.h"
#include "ActiveSessionSocket.moc"

namespace Aesalon {
namespace GUI {

ActiveSessionSocket::ActiveSessionSocket(QString host, int port, Platform::Memory *memory) : memory(memory) {
    socket = new QTcpSocket();
    
    socket->connectToHost(host, port);
    connect(socket, SIGNAL(readyRead()), this, SLOT(handle_data()));
    connect(socket, SIGNAL(connected()), this, SLOT(reemit_connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(reemit_disconnected()));
}

ActiveSessionSocket::~ActiveSessionSocket() {
}

void ActiveSessionSocket::handle_data() {
    QByteArray data = socket->readAll();
    QString str = data;
    while(data.length()) {
        QString string = data;
        data.remove(0, string.length()+1);
        Platform::Event *event = Platform::Event::deserialize(string.toStdString());
        if(event) {
            memory->handle_event(event);
            emit event_received(event);
            delete event;
        }
    }
}

} // namespace GUI
} // namespace Aesalon
