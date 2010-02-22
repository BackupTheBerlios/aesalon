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
    QTimer *request_queue_timer;
    
    Timestamp *start_time, *finish_time;
public:
    DataThread(QObject *parent, DataSource *data_source);
    virtual ~DataThread();
    
    DataRequestQueue *get_request_queue() const { return request_queue; }
    void event_received(Event *event);
    
    const Timestamp *get_start_time() const { return start_time; }
    const Timestamp *get_finish_time() const { return finish_time; }
    
    SnapshotList *get_snapshot_list() { return &snapshot_list; }
private slots:
    void create_new_snapshot();
    void started();
    void finished();
    void process_request_queue();
protected:
    virtual void run();
signals:
    void data_started();
    void data_finished();
};

#endif
