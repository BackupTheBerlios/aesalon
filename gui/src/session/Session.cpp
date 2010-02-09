#include "Session.h"
#include "Session.moc"
#include "storage/StorageFactory.h"

Session::Session(QWidget *parent, DataSource *data_source) : QWidget(parent), data_source(data_source) {
    data_receiver = data_source->spawn_receiver();
    data_receiver->start();
    current_memory = snapshot_list.append_snapshot();
}

Session::~Session() {
    data_receiver->terminate();
}

void Session::handle_event(Event *event) {
    event->apply_to(current_memory);
    current_memory->add_event(event);
}
