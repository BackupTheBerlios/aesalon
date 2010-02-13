#ifndef AESALON_GUI_SESSION_DATA_THREAD_H
#define AESALON_GUI_SESSION_DATA_THREAD_H

#include <QThread>
#include <QTimer>

#include "storage/SnapshotList.h"
#include "data/DataSource.h"

class DataThread : public QThread { Q_OBJECT
private:
    DataSource *data_source;
    DataReceiver *data_receiver;
    
    SnapshotList snapshot_list;
    Snapshot *current_snapshot;
public:
    DataThread(QObject *parent, DataSource *data_source);
    virtual ~DataThread();
private slots:
    void event_received(Event *event);
    void create_new_snapshot();
protected:
    virtual void run();
};

#endif
