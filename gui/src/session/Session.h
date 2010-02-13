#ifndef AESALON_GUI_SESSION_H
#define AESALON_GUI_SESSION_H

#include <QWidget>
#include <QThread>
#include "data/DataSource.h"
#include "data/DataReceiver.h"
#include "storage/SnapshotList.h"
#include "DataThread.h"

class Session : public QWidget { Q_OBJECT
private:
    DataSource *data_source;
    DataThread *data_thread;
public:
    Session(QWidget *parent, DataSource *data_source);
    virtual ~Session();
signals:
    void current_memory_snapshot_changed(Snapshot *new_current_memory);
};

#endif
