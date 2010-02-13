#ifndef AESALON_GUI_DATA_RECEIVER_H
#define AESALON_GUI_DATA_RECEIVER_H

#include <QThread>
#include "storage/Event.h"

class DataReceiver : public QObject { Q_OBJECT
public:
    DataReceiver(QObject *parent) : QObject(parent) {}
    virtual ~DataReceiver() {}
signals:
    void event_received(Event *event);
    void no_more_data();
};

#endif
