#include "DataThread.h"
#include "DataThread.moc"
#include "VisualizationThread.h"

DataThread::DataThread(QObject *parent, DataSource *data_source) : QThread(parent), data_source(data_source) {
    request_queue = new DataRequestQueue();
    current_snapshot = snapshot_list.append_snapshot();
    current_snapshot->set_head_node(new BiTreeNode(current_snapshot->get_snapshot_id()));
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
    connect(data_receiver, SIGNAL(started(Timestamp *)), SLOT(started(Timestamp *)));
    connect(data_receiver, SIGNAL(finished(Timestamp *)), SLOT(finished(Timestamp *)));
    snapshot_timer = new QTimer();
    connect(snapshot_timer, SIGNAL(timeout()), this, SLOT(create_new_snapshot()));
    request_queue_timer = new QTimer();
    request_queue_timer->setSingleShot(false);
    connect(request_queue_timer, SIGNAL(timeout()), this, SLOT(process_request_queue()));
    request_queue_timer->start(500);
    exec();
    delete data_receiver;
    delete snapshot_timer;
}

void DataThread::event_received(Event *event) {
    Snapshot *last = get_snapshot_list()->get_snapshot(current_snapshot->get_snapshot_id()-1);
    if(last->get_event_list()->get_event_list().size() > 1000) create_new_snapshot();
    last->add_event(event);
    event->apply_to(current_snapshot);
}

void DataThread::create_new_snapshot() {
    current_snapshot->update_timestamp(Timestamp());
    current_snapshot = snapshot_list.append_snapshot();
}

void DataThread::started(Timestamp *time) {
    /* NOTE: get this from somewhere else . . . hardcoding it is a bad idea. */
    snapshot_timer->start(5000);
    start_time = time;
    snapshot_list.get_snapshot(1)->update_timestamp(*start_time);
    current_snapshot->update_timestamp(*start_time);
    emit data_started();
}

void DataThread::finished(Timestamp *time) {
    if(finish_time) return;
    snapshot_timer->stop();
    /*current_snapshot = snapshot_list.append_snapshot();*/
    finish_time = time;
    emit data_finished();
}

void DataThread::process_request_queue() {
    while(request_queue->current_requests()) {
        DataRequest *request = request_queue->pop_request();
        request->gather_data(this);
        request->get_v_thread()->get_request_queue()->push_request(request);
    }
}
