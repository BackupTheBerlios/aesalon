#include "DataThread.h"
#include "DataThread.moc"

DataThread::DataThread(QObject *parent, DataSource *data_source) : QThread(parent), data_source(data_source) {
}

DataThread::~DataThread() {

}

void DataThread::run() {
    data_receiver = data_source->spawn_receiver(NULL);
    connect(data_receiver, SIGNAL(event_received(Event*)), SLOT(event_received(Event*)));
    current_snapshot = snapshot_list.append_snapshot();
    QTimer snapshot_timer;
    connect(&snapshot_timer, SIGNAL(timeout()), this, SLOT(create_new_snapshot()));
    /* NOTE: get this from somewhere else . . . hardcoding it is a bad idea. */
    snapshot_timer.start(5000);
    exec();
    delete data_receiver;
}

void DataThread::event_received(Event *event) {
    current_snapshot->add_event(event);
}

void DataThread::create_new_snapshot() {
    current_snapshot = snapshot_list.append_snapshot();
}
