#include "ActiveSession.h"
#include "ActiveSession.moc"

namespace Aesalon {
namespace GUI {

ActiveSession::ActiveSession(Session *session, QWidget *parent) : QTabWidget(parent), session(session), status(INITIALIZING) {
    this->setTabPosition(West);
    overview = new ActiveSessionOverview(session);
    this->addTab(overview, tr("&Overview"));
    
    memory = new Platform::Memory();
    status = WAITING_FOR_CONNECTION;
    connect(this, SIGNAL(status_changed(QString)), overview, SLOT(update_status(QString)));
    emit status_changed(get_status_as_string());
    
    block_view = new ActiveSessionBlockView(memory);
    this->addTab(block_view, tr("&Block view"));
}

ActiveSession::~ActiveSession() {
    delete memory;
}

void ActiveSession::execute() {
    connect_to("localhost", session->get_port());
}

void ActiveSession::connect_to(QString host, int port) {
    socket = new ActiveSessionSocket(host, port, memory);
    connect(socket, SIGNAL(connected()), this, SLOT(socket_connection()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socket_disconnection()));
    
    connect(socket, SIGNAL(event_received(Platform::Event*)), block_view, SLOT(memory_changed(Platform::Event*)));
}

void ActiveSession::socket_connection() {
    set_status(CONNECTED);
}

void ActiveSession::socket_disconnection() {
    set_status(CONNECTION_CLOSED);
}

QString ActiveSession::get_status_as_string() const {
    switch(get_status()) {
        case INITIALIZING:
            return tr("Initializing . . .");
        case WAITING_FOR_CONNECTION:
            return tr("Establishing connection to monitor . . .");
        case CONNECTED:
            return tr("Connection established to monitor.");
        case CONNECTION_CLOSED:
            return tr("Connection to monitor closed.");
    }
    qDebug("Invalid ActiveSession status!");
    return tr("Invalid ActiveSession status!");
}

} // namespace GUI
} // namespace Aesalon
