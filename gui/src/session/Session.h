#ifndef AESALON_GUI_SESSION_H
#define AESALON_GUI_SESSION_H

#include <QWidget>
#include <QThread>
#include "data/DataSource.h"
#include "data/DataReceiver.h"
#include "storage/SnapshotList.h"

class Session : public QWidget { Q_OBJECT
private:
    DataSource *data_source;
    DataReceiver *data_receiver;
    SnapshotList snapshot_list;
    Snapshot *current_memory;
public:
    Session(QWidget *parent, DataSource *data_source);
    virtual ~Session();
private slots:
    void handle_event(Event *event);
signals:
    void current_memory_snapshot_changed(Snapshot *new_current_memory);
};

#endif
