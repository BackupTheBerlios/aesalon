#ifndef AESALON_GUI_SESSION_DATA_THREAD_H
#define AESALON_GUI_SESSION_DATA_THREAD_H

#include <QThread>
#include <QTimer>
#include <QQueue>

#include "storage/SnapshotList.h"
#include "data/DataSource.h"
#include "DataRequest.h"
#include "DataRequestQueue.h"

class DataThread : public QThread { Q_OBJECT
private:
    DataSource *data_source;
    DataReceiver *data_receiver;
    
    SnapshotList snapshot_list;
    Snapshot *current_snapshot;
    QTimer *snapshot_timer;
    DataRequestQueue *request_queue;
public:
    DataThread(QObject *parent, DataSource *data_source);
    virtual ~DataThread();
    
    DataRequestQueue *get_request_queue() const { return request_queue; }
    void event_received(Event *event);
private slots:
    void create_new_snapshot();
    void begun();
    void finished();
protected:
    virtual void run();
};

#endif
