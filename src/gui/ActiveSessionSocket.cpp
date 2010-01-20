#include "ActiveSessionSocket.h"
#include "ActiveSessionSocket.moc"

namespace Aesalon {
namespace GUI {

ActiveSessionSocket::ActiveSessionSocket(QString host, int port, Platform::Memory *memory) : memory(memory) {
    socket = new QTcpSocket();
    
    socket->connectToHost(host, port);
    connect(socket, SIGNAL(readyRead()), this, SLOT(handle_data()));
    connect(socket, SIGNAL(connected()), this, SLOT(reemit_connected()));
}

ActiveSessionSocket::~ActiveSessionSocket() {
}

void ActiveSessionSocket::handle_data() {
    QByteArray data = socket->readAll();
    while(data.length()) {
        QString string = data;
        data.remove(0, string.length());
        Platform::Event *event = Platform::Event::deserialize(string.toStdString());
        if(event) memory->handle_event(event);
    }
}

} // namespace GUI
} // namespace Aesalon
