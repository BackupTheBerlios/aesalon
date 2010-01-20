#ifndef AESALON_GUI_ACTIVE_SESSION_H
#define AESALON_GUI_ACTIVE_SESSION_H

#include <QWidget>
#include <QTabWidget>

#include "Session.h"

#include "platform/Memory.h"

namespace Aesalon {
namespace GUI {

class ActiveSession : public QTabWidget { Q_OBJECT
private:
    Session *session;
    Platform::Memory *memory;
public:
    ActiveSession(Session *session, QWidget *parent = 0);
    virtual ~ActiveSession();

    void execute();
public slots:
    void terminate_session() { close_session(this); }
signals:
    void close_session(ActiveSession *session);
};

} // namespace GUI
} // namespace Aesalon

#endif
