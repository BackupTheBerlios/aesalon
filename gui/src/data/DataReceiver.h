#ifndef AESALON_GUI_DATA_RECEIVER_H
#define AESALON_GUI_DATA_RECEIVER_H

#include <QThread>
#include "storage/Event.h"

class DataThread;

class DataReceiver : public QObject { Q_OBJECT
private:
    DataThread *data_thread;
public:
    DataReceiver(DataThread *data_thread) : QObject(NULL), data_thread(data_thread) {}
    virtual ~DataReceiver() {}
    
    void event_received(Event *event);
signals:
    void begun();
    void finished();
};

#endif
