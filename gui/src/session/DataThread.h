#ifndef AESALON_GUI_SESSION_DATA_THREAD_H
#define AESALON_GUI_SESSION_DATA_THREAD_H

#include <QThread>
#include <QTimer>

#include "storage/SnapshotList.h"
#include "data/DataSource.h"
#include "DataRequest.h"

class DataThread : public QThread { Q_OBJECT
private:
    DataSource *data_source;
    DataReceiver *data_receiver;
    
    SnapshotList snapshot_list;
    Snapshot *current_snapshot;
    QTimer *snapshot_timer;
public:
    DataThread(QObject *parent, DataSource *data_source);
    virtual ~DataThread();
    
    void push_request(DataRequest *request);
private slots:
    void event_received(Event *event);
    void create_new_snapshot();
    void no_more_data();
protected:
    virtual void run();
};

#endif
