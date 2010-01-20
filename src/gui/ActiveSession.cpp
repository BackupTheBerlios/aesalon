#include "ActiveSession.h"
#include "ActiveSession.moc"
#include "ActiveSessionOverview.h"

namespace Aesalon {
namespace GUI {

ActiveSession::ActiveSession(Session *session, QWidget *parent) : QTabWidget(parent), session(session) {
    this->setTabPosition(West);
    this->addTab(new ActiveSessionOverview(session), "&Overview");
    
    memory = new Platform::Memory();
}

ActiveSession::~ActiveSession() {
    memory_mutex.lock();
    delete memory;
    memory_mutex.unlock();
}

void ActiveSession::execute() {
    connect_to("localhost", session->get_port());
    /*emit close_session(this);*/
}

void ActiveSession::connect_to(QString host, int port) {
    socket = new ActiveSessionSocket(host, port, memory, &memory_mutex);
    socket->start();
    connect(socket, SIGNAL(finished()), this, SLOT(terminate_session()));
}

} // namespace GUI
} // namespace Aesalon
