#ifndef AESALON_GUI_ACTIVE_SESSION_H
#define AESALON_GUI_ACTIVE_SESSION_H

#include <QWidget>
#include <QTabWidget>

#include "Session.h"
#include "ActiveSessionSocket.h"
#include "ActiveSessionOverview.h"

#include "platform/Memory.h"

namespace Aesalon {
namespace GUI {

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
    
    Platform::Memory *memory;
    ActiveSessionSocket *socket;
    ActiveSessionOverview *overview;
    
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
    
    Platform::Memory *get_memory() const { return memory; }
    
    status_e get_status() const { return status; }
    QString get_status_as_string() const;
    
    void connect_to(QString host, int port);
public slots:
    void terminate_session() { emit close_session(this); }
    void socket_connection();
signals:
    void close_session(ActiveSession *session);
    void status_changed(QString new_status);
};

} // namespace GUI
} // namespace Aesalon

#endif
