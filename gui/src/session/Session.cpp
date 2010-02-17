#include "Session.h"
#include "Session.moc"

Session::Session(QWidget *parent, DataSource *data_source) : QWidget(parent), data_source(data_source) {
    data_thread = new DataThread(this, data_source);
    data_thread->start();
}

Session::~Session() {
    data_thread->quit();
    data_thread->wait();
}
