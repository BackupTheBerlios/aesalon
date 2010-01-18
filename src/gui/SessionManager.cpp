#include <iostream>
#include <QSettings>
#include "SessionManager.h"
#include "SessionManager.moc"
#include "SessionEditor.h"

namespace Aesalon {
namespace GUI {

SessionManager::SessionManager(QWidget *parent) {
    QSettings settings;
    
    int sessions = settings.beginReadArray("sessions");
    for(int i = 0; i < sessions; i ++) {
        settings.setArrayIndex(i);
        Session *session = new Session();
        session->set_session_name(settings.value("session-name").toString());
        session->set_executable_path(settings.value("executable-path").toString());
        session_list.append(session);
    }
    settings.endArray();
    
    layout = new QVBoxLayout();
    
    session_list_view = new SessionListWidget();
    session_list_view->update_list(session_list);
    layout->addWidget(session_list_view);
    
    
    session_button_layout = new QHBoxLayout();
    
    new_session_button = new QPushButton("&New session");
    connect(new_session_button, SIGNAL(pressed()), this, SLOT(new_session()));
    session_button_layout->addWidget(new_session_button);
    
    edit_session_button = new QPushButton("&Edit session");
    connect(edit_session_button, SIGNAL(pressed()), this, SLOT(edit_current_session()));
    session_button_layout->addWidget(edit_session_button);
    
    launch_session_button = new QPushButton("&Launch session");
    connect(launch_session_button, SIGNAL(pressed()), this, SLOT(launch_current_session()));
    session_button_layout->addWidget(launch_session_button);
    
    layout->addLayout(session_button_layout);
    this->setLayout(layout);
    connect(session_list_view, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(edit_session(QListWidgetItem *)));
}

SessionManager::~SessionManager() {
    save_sessions();
}

void SessionManager::save_sessions() {
    QSettings settings;
    settings.beginWriteArray("sessions");
    for(int i = 0; i < session_list.size(); i ++) {
        settings.setArrayIndex(i);
        settings.setValue("session-name", session_list[i]->get_session_name());
        settings.setValue("executable-path", session_list[i]->get_executable_path());
    }
    settings.endArray();
}

Session *SessionManager::get_session_by_name(const QString &name) const {
    for(int i = 0; i < session_list.size(); i ++) {
        if(session_list[i]->get_session_name() == name) return session_list[i];
    }
    return NULL;
}

void SessionManager::new_session() {
    SessionEditor *se = new SessionEditor(this, NULL);
    se->exec();
    session_list.append(se->get_session());
    session_list_view->update_list(session_list);
}

void SessionManager::launch_session(QListWidgetItem *session_item) {
    QString session_name = session_item->data(Qt::DisplayRole).toString();
    Session *session = get_session_by_name(session_name);
}

void SessionManager::edit_session(QListWidgetItem *session_item) {
    QString session_name = session_item->data(Qt::DisplayRole).toString();
    Session *session = get_session_by_name(session_name);
    SessionEditor *se = new SessionEditor(this, session);
    se->exec();
    session_list_view->update_list(session_list);
}

} // namespace GUI
} // namespace Aesalon
