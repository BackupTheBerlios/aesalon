#ifndef AESALON_GUI_SESSION_OVERVIEW_H
#define AESALON_GUI_SESSION_OVERVIEW_H

#include <QWidget>
#include <QLabel>
#include <QFormLayout>
#include <QPushButton>
#include "storage/Timestamp.h"
#include "VisualizationList.h"

class SessionOverview : public QWidget { Q_OBJECT
private:
    enum {
        WAITING,
        RUNNING,
        FINISHED
    } status;
    Timestamp start_timestamp;
    Timestamp finish_timestamp;
    DataThread *data_thread;
    
    QFormLayout *main_layout;
    QLabel *time_running_label;
    QPushButton *new_visualization_button;
    VisualizationList *v_list;
public:
    SessionOverview(DataThread *data_thread);
    ~SessionOverview();
private slots:
    void update_time_running();
    void new_visualization();
public slots:
    void started();
    void finished();
signals:
    void new_visualization(Visualization *visualization);
};

#endif
