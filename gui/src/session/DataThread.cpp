#include "DataThread.h"
#include "DataThread.moc"

DataThread::DataThread(QObject *parent, DataSource *data_source) : QThread(parent), data_source(data_source) {
    request_queue = new DataRequestQueue();
    current_snapshot = snapshot_list.append_snapshot();
}

DataThread::~DataThread() {

}

void DataThread::run() {
    data_receiver = data_source->spawn_receiver(this);
    connect(data_receiver, SIGNAL(started()), SLOT(started()));
    connect(data_receiver, SIGNAL(finished()), SLOT(finished()));
    snapshot_timer = new QTimer();
    connect(snapshot_timer, SIGNAL(timeout()), this, SLOT(create_new_snapshot()));
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
}

void DataThread::finished() {
    snapshot_timer->stop();
    /*current_snapshot = snapshot_list.append_snapshot();*/
}
