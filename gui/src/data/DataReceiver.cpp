#include "session/DataThread.h"
#include "DataReceiver.h"
#include "DataReceiver.moc"
#include "session/DataThread.h"

void DataReceiver::event_received(Event *event) {
    data_thread->event_received(event);
}
