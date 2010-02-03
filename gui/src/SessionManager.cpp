#include <iostream>
#include <QSettings>
#include "SessionManager.h"
#include "SessionManager.moc"
#include "SessionEditor.h"
#include "ActiveSession.h"
#include "MainArea.h"

SessionManager::SessionManager(QWidget *parent) {
    QSettings settings;
    
    int sessions = settings.beginReadArray("sessions");
    for(int i = 0; i < sessions; i ++) {
        settings.setArrayIndex(i);
        Session *session = new Session();
        session->set_session_name(settings.value("session-name").toString());
        session->set_executable_path(settings.value("executable-path").toString());
        session->set_arguments(settings.value("arguments").toString());
        session->set_port(settings.value("port").toInt());
        session->set_snapshot_interval(settings.value("snapshot-interval").toInt());
        session->set_snapshot_interval(settings.value("full-snapshot-interval").toInt());
        session->set_session_type(Session::session_type_e(settings.value("session-type").toInt()));
        session_list.append(session);
    }
    settings.endArray();
    
    layout = new QVBoxLayout();
    
    session_list_view = new SessionListWidget();
    session_list_view->update_list(session_list);
    layout->addWidget(session_list_view);
    
    if(sessions) session_list_view->setCurrentRow(0);
    
    session_button_layout = new QHBoxLayout();
    
    new_session_button = new QPushButton("&New session");
    connect(new_session_button, SIGNAL(pressed()), this, SLOT(new_session()));
    session_button_layout->addWidget(new_session_button);
    
    launch_session_button = new QPushButton("&Launch session");
    connect(launch_session_button, SIGNAL(pressed()), this, SLOT(launch_current_session()));
    session_button_layout->addWidget(launch_session_button);
    
    layout->addLayout(session_button_layout);
    this->setLayout(layout);
    
    session_list_view->setFocus(Qt::ActiveWindowFocusReason);
    
    connect(session_list_view, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(edit_session(QListWidgetItem *)));
    connect(session_list_view, SIGNAL(new_request()), this, SLOT(new_session()));
    connect(session_list_view, SIGNAL(edit_request(QListWidgetItem*)), this, SLOT(edit_session(QListWidgetItem*)));
    connect(session_list_view, SIGNAL(delete_request(QListWidgetItem*)), this, SLOT(delete_session(QListWidgetItem*)));
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
        settings.setValue("arguments", session_list[i]->get_arguments());
        settings.setValue("port", session_list[i]->get_port());
        settings.setValue("snapshot-interval", session_list[i]->get_snapshot_interval());
        settings.setValue("full-snapshot-interval", session_list[i]->get_full_snapshot_interval());
        settings.setValue("session-type", session_list[i]->get_session_type());
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
    if(se->exec() == QDialog::Accepted) {
        session_list.append(se->get_session());
        session_list_view->update_list(session_list);
    }
}

void SessionManager::launch_session(QListWidgetItem *session_item) {
    if(session_item == NULL) return;
    QString session_name = session_item->data(Qt::DisplayRole).toString();
    Session *session = get_session_by_name(session_name);
    
    ActiveSession *as = new ActiveSession(session);
    emit new_tab_request(as, session_name);
    connect(as, SIGNAL(close_session(ActiveSession *)), this, SLOT(close_active_session(ActiveSession *)));
    if(session->get_session_type() == Session::LAUNCH_SESSION) as->execute();
    else as->connect_to(session->get_executable_path(), session->get_port());
}

void SessionManager::edit_session(QListWidgetItem *session_item) {
    QString session_name = session_item->data(Qt::DisplayRole).toString();
    Session *session = get_session_by_name(session_name);
    SessionEditor *se = new SessionEditor(this, session);
    se->exec();
    session_list_view->update_list(session_list);
}

void SessionManager::delete_session(QListWidgetItem *session_item) {
    if(session_item == NULL) return;
    QString session_name = session_item->data(Qt::DisplayRole).toString();
    Session *session = get_session_by_name(session_name);
    if(session_list.removeOne(session)) session_list_view->update_list(session_list);
}

void SessionManager::close_active_session(ActiveSession *session) {
    emit remove_tab_request(session);
}
