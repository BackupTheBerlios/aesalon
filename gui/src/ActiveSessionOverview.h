#ifndef AESALON_GUI_ACTIVE_SESSION_OVERVIEW_H
#define AESALON_GUI_ACTIVE_SESSION_OVERVIEW_H

#include <QWidget>
#include <QFormLayout>
#include <QLabel>
#include <QTime>
#include <QTimer>

#include "Session.h"
#include "ActiveSessionMemory.h"

class ActiveSessionOverview : public QWidget { Q_OBJECT
private:
    Session *session;
    QFormLayout *info_form;
    QLabel *status;
    QLabel *allocation_info;
    QLabel *active_blocks;
public:
    ActiveSessionOverview(Session *session, QWidget *parent = 0);
    virtual ~ActiveSessionOverview() {}
public slots:
    void update_status(QString new_status);
    void memory_changed(ActiveSessionMemorySnapshot *snapshot);
};

#endif