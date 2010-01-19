#ifndef AESALON_GUI_ACTIVE_SESSION_OVERVIEW_H
#define AESALON_GUI_ACTIVE_SESSION_OVERVIEW_H

#include <QWidget>
#include <QFormLayout>
#include <QLabel>
#include <QTime>
#include <QTimer>

#include "Session.h"

namespace Aesalon {
namespace GUI {

class ActiveSessionOverview : public QWidget { Q_OBJECT
private:
    Session *session;
    QFormLayout *info_form;
    QTime run_time;
    QLabel *running_time;
    QTimer *running_time_timer;
public:
    ActiveSessionOverview(Session *session, QWidget *parent = 0);
    virtual ~ActiveSessionOverview() {}
public slots:
    void update_running_time();
};

} // namespace GUI
} // namespace Aesalon

#endif