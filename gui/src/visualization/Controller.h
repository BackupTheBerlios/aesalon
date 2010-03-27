#ifndef AESALON_GUI_VISUALIZATION_CONTROLLER_H
#define AESALON_GUI_VISUALIZATION_CONTROLLER_H

#include <QThread>
#include <QTimer>

#include "session/DataThread.h"

class Controller : public QThread {
private:
    DataThread *data_thread;
    QTimer *update_timer;
public:
    Controller(DataThread *data_thread);
    virtual ~Controller();
    
    virtual void run();
public slots:
    void change_update_time(int ms);
private slots:
    void update();
};

#endif
