#include "Session.h"
#include "Session.moc"
#include "storage/StorageFactory.h"

Session::Session(QWidget *parent, DataSource *data_source) : QWidget(parent), data_source(data_source) {
    data_receiver = data_source->spawn_receiver(this);
    connect(data_receiver, SIGNAL(event_received(Event*)), SLOT(handle_event(Event*)));
    data_receiver->start();
    current_memory = snapshot_list.append_snapshot();
}

Session::~Session() {
    data_receiver->quit();
    data_receiver->wait();
}

void Session::handle_event(Event *event) {
    event->apply_to(current_memory);
    current_memory->add_event(event);
}
