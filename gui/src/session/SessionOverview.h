#ifndef AESALON_GUI_SESSION_OVERVIEW_H
#define AESALON_GUI_SESSION_OVERVIEW_H

#include <QWidget>
#include <QLabel>
#include <QFormLayout>
#include <QPushButton>
#include "storage/Timestamp.h"

class SessionOverview : public QWidget { Q_OBJECT
private:
    enum {
        WAITING,
        RUNNING,
        FINISHED
    } status;
    Timestamp start_timestamp;
    Timestamp finish_timestamp;
    
    QFormLayout *main_layout;
    QLabel *time_running_label;
    QPushButton *new_visualization_button;
public:
    SessionOverview(QWidget* parent = 0);
    ~SessionOverview();
private slots:
    void update_time_running();
    void new_visualization();
public slots:
    void started();
    void finished();
};

#endif
