#ifndef AESALON_GUI_DATA_RECEIVER_H
#define AESALON_GUI_DATA_RECEIVER_H

#include <QThread>
#include "storage/Event.h"

class DataReceiver : public QThread { Q_OBJECT
public:
    DataReceiver(QObject *parent) : QThread(parent) {}
    virtual ~DataReceiver() {}
protected:
    virtual void run() = 0;
signals:
    void event_received(Event *event);
    void no_more_data();
};

#endif
