#include <sys/types.h>
#include <unistd.h>

#include <QSettings>
#include <QShortcut>

#include "ActiveSession.h"
#include "ActiveSession.moc"

#include "platform/ArgumentList.h"

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
    
    QShortcut *close_tab = new QShortcut(Qt::Key_W + Qt::CTRL, this);
    connect(close_tab, SIGNAL(activated()), this, SLOT(terminate_session()));
}

ActiveSession::~ActiveSession() {
    delete memory;
}

void ActiveSession::execute() {
    QSettings settings;
    Platform::ArgumentList al;
    al.add_argument(settings.value("aesalon-path").toString().toStdString());
    al.add_argument("--wait");
    al.add_argument("--use-port");
    al.add_argument(QString().setNum(session->get_port()).toStdString());
    al.add_argument(session->get_executable_path().toStdString());
    /* TODO: handle arguments in here . . . */
    pid_t pid = fork();
    if(pid == -1) return;
    else if(pid == 0) {
        execv(al.get_argument(0).c_str(), al.get_as_argv());
        exit(1);
    }
    connect_to("localhost", session->get_port());
}

void ActiveSession::connect_to(QString host, int port) {
    socket = new ActiveSessionSocket(host, port, memory);
    connect(socket, SIGNAL(connected()), this, SLOT(socket_connection()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socket_disconnection()));
    
    connect(socket, SIGNAL(event_received(Platform::Event*)), block_view, SLOT(memory_changed(Platform::Event*)));
    connect(socket, SIGNAL(event_received(Platform::Event*)), overview, SLOT(handle_event(Platform::Event*)));
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
            return tr("Connecting . . .");
        case CONNECTED:
            return tr("Running . . .");
        case CONNECTION_CLOSED:
            return tr("Execution completed.");
    }
    qDebug("Invalid ActiveSession status!");
    return tr("Invalid ActiveSession status!");
}

} // namespace GUI
} // namespace Aesalon
