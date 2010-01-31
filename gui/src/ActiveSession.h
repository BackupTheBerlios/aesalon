#ifndef AESALON_GUI_ACTIVE_SESSION_H
#define AESALON_GUI_ACTIVE_SESSION_H

#include <QWidget>
#include <QTabWidget>
#include <QDateTime>

#include "Session.h"
#include "ActiveSessionSocket.h"
#include "ActiveSessionOverview.h"
#include "ActiveSessionBlockView.h"
#include "ActiveSessionMemory.h"

class ActiveSession : public QTabWidget { Q_OBJECT
public:
    enum status_e {
        INITIALIZING,
        WAITING_FOR_CONNECTION,
        CONNECTED,
        CONNECTION_CLOSED
    };
private:
    Session *session;
    
    ActiveSessionSocket *socket;
    ActiveSessionOverview *overview;
    ActiveSessionBlockView *block_view;
    ActiveSessionMemory *memory;
    
    QDateTime start_time;
    
    status_e status;
    void set_status(status_e new_status) {
        if(new_status == status) return;
        status = new_status;
        emit status_changed(get_status_as_string());
    }
public:
    ActiveSession(Session *session, QWidget *parent = 0);
    virtual ~ActiveSession();

    void execute();
    
    status_e get_status() const { return status; }
    QString get_status_as_string() const;
    
    QDateTime get_start_time() const { return start_time; }
    
    void connect_to(QString host, int port);
public slots:
    void terminate_session() { emit close_session(this); }
    void socket_connection();
    void socket_disconnection();
signals:
    void close_session(ActiveSession *session);
    void status_changed(QString new_status);
};

#endif
