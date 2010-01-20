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
}

ActiveSession::~ActiveSession() {
    delete memory;
}

void ActiveSession::execute() {
    connect_to("localhost", session->get_port());
}

void ActiveSession::connect_to(QString host, int port) {
    socket = new ActiveSessionSocket(host, port, memory);
}

void ActiveSession::socket_connection() {
    set_status(CONNECTED);
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
