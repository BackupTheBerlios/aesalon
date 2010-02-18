#include "DataThread.h"
#include "DataThread.moc"

DataThread::DataThread(QObject *parent, DataSource *data_source) : QThread(parent), data_source(data_source) {
    request_queue = new DataRequestQueue();
}

DataThread::~DataThread() {

}

void DataThread::run() {
    data_receiver = data_source->spawn_receiver(NULL);
    connect(data_receiver, SIGNAL(event_received(Event*)), SLOT(event_received(Event*)));
    connect(data_receiver, SIGNAL(no_more_data()), SLOT(no_more_data()));
    current_snapshot = snapshot_list.append_snapshot();
    snapshot_timer = new QTimer();
    connect(snapshot_timer, SIGNAL(timeout()), this, SLOT(create_new_snapshot()));
    /* NOTE: get this from somewhere else . . . hardcoding it is a bad idea. */
    snapshot_timer->start(5000);
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

void DataThread::no_more_data() {
    snapshot_timer->stop();
    /*current_snapshot = snapshot_list.append_snapshot();*/
}
