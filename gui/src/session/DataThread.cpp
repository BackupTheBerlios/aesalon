#include "DataThread.h"
#include "DataThread.moc"
#include "VisualizationThread.h"

DataThread::DataThread(QObject *parent, DataSource *data_source) : QThread(parent), data_source(data_source) {
    request_queue = new DataRequestQueue();
    current_snapshot = snapshot_list.append_snapshot();
    start_time = NULL;
    finish_time = NULL;
}

DataThread::~DataThread() {
    if(start_time) delete start_time;
    if(finish_time) delete finish_time;
}

void DataThread::run() {
    data_receiver = data_source->spawn_receiver(this);
    connect(data_receiver, SIGNAL(started()), SLOT(started()));
    connect(data_receiver, SIGNAL(finished()), SLOT(finished()));
    snapshot_timer = new QTimer();
    connect(snapshot_timer, SIGNAL(timeout()), this, SLOT(create_new_snapshot()));
    request_queue_timer = new QTimer();
    connect(request_queue_timer, SIGNAL(timeout()), this, SLOT(process_request_queue()));
    request_queue_timer->start(500);
    exec();
    delete data_receiver;
    delete snapshot_timer;
}

void DataThread::event_received(Event *event) {
    event->apply_to(current_snapshot);
    current_snapshot->add_event(event);
}

void DataThread::create_new_snapshot() {
    current_snapshot = snapshot_list.append_snapshot();
}

void DataThread::started() {
    /* NOTE: get this from somewhere else . . . hardcoding it is a bad idea. */
    snapshot_timer->start(5000);
    start_time = new Timestamp();
    emit data_started();
}

void DataThread::finished() {
    snapshot_timer->stop();
    current_snapshot = snapshot_list.append_snapshot();
    finish_time = new Timestamp();
    emit data_finished();
}

void DataThread::process_request_queue() {
    request_queue_timer->stop();
    qDebug("DataThread: processing request queue . . .");
    
    while(request_queue->current_requests()) {
        DataRequest *request = request_queue->pop_request();
        request->gather_data(this);
        request->get_v_thread()->get_request_queue()->push_request(request);
    }
    
    qDebug("DataThread: restarting request queue timer . . .");
    request_queue_timer->start();
}
