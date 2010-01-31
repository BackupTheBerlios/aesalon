#include <sys/types.h>
#include <unistd.h>
#include <cstring>

#include <QSettings>
#include <QShortcut>

#include "ActiveSession.h"
#include "ActiveSession.moc"

ActiveSession::ActiveSession(Session *session, QWidget *parent) : QTabWidget(parent), session(session), status(INITIALIZING) {
    this->setTabPosition(West);
    overview = new ActiveSessionOverview(session);
    this->addTab(overview, tr("&Overview"));
    block_view = new ActiveSessionBlockView(this);
    this->addTab(block_view, tr("Current &blocks"));
    
    status = WAITING_FOR_CONNECTION;
    connect(this, SIGNAL(status_changed(QString)), overview, SLOT(update_status(QString)));
    emit status_changed(get_status_as_string());
    
    QShortcut *close_tab = new QShortcut(Qt::Key_W + Qt::CTRL, this);
    connect(close_tab, SIGNAL(activated()), this, SLOT(terminate_session()));
    
    start_time = QDateTime::currentDateTime();
    
    memory = new ActiveSessionMemory(this);
    
    connect(memory, SIGNAL(memory_changed(ActiveSessionMemorySnapshot*)), overview, SLOT(memory_changed(ActiveSessionMemorySnapshot*)));
    connect(memory, SIGNAL(memory_changed(ActiveSessionMemorySnapshot*)), block_view, SLOT(update_content(ActiveSessionMemorySnapshot*)));
}

ActiveSession::~ActiveSession() {
}

void ActiveSession::execute() {
    QSettings settings;
    
    QString command = "";
    /* NOTE: reimplement this function */

    command += settings.value("xterm-path").toString();
    command += " -e ";
    command += settings.value("aesalon-path").toString();
    command += " --wait --tcp-port ";
    command += QString().setNum(session->get_port());
    command += " ";
    command += session->get_executable_path();
    command += " ";
    command += session->get_arguments();
    
    int argc = command.count(' ') + 1;
    char **argv = new char*[argc];
    
    argv[argc] = 0;
    
    for(int x = 0; x < argc; x ++) {
        QString arg = command.left(command.indexOf(' '));
        command.remove(0, command.indexOf(' ')+1);
        argv[x] = new char[arg.size()+1];
        std::strcpy(argv[x], arg.toStdString().c_str());
    }
    
    pid_t pid = fork();
    if(pid == 0) {
        execv(argv[0], argv);
        qDebug("Couldn't execute file \"%s\"", argv[0]);
        exit(1);
    }
    
    for(int x = 0; x < argc; x ++) {
        delete argv[x];
    }
    delete[] argv;
    
    connect_to("localhost", session->get_port());
}

void ActiveSession::connect_to(QString host, int port) {
    socket = new ActiveSessionSocket(host, port);
    connect(socket, SIGNAL(connected()), this, SLOT(socket_connection()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socket_disconnection()));
    connect(socket, SIGNAL(received_data(QByteArray)), memory, SLOT(process_data(QByteArray)));
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
