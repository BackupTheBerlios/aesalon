#ifndef AESALON_GUI_ACTIVE_SESSION_H
#define AESALON_GUI_ACTIVE_SESSION_H

#include <QWidget>

#include "Session.h"

namespace Aesalon {
namespace GUI {

class ActiveSession : public QWidget { Q_OBJECT
private:
    Session *session;
public:
    ActiveSession(Session *session, QWidget *parent = 0);
    virtual ~ActiveSession() {}

    void execute();
signals:
    void close_session(ActiveSession *session);
};

} // namespace GUI
} // namespace Aesalon

#endif
