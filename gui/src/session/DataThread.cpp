/** Aesalon, a project to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009-2010 strange <kawk256@gmail.com>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    @file session/DataThread.cpp
*/

#include "DataThread.h"
#include "DataThread.moc"

DataThread::DataThread(QObject *parent, DataSource *data_source) : QThread(parent), data_source(data_source) {
    current_snapshot = snapshot_list.append_snapshot();
    current_snapshot->set_head_node(new BiTreeNode(current_snapshot->get_snapshot_id()));
    current_snapshot->update_timestamp(Timestamp(0));
    current_snapshot = snapshot_list.append_snapshot();
    current_snapshot->update_timestamp(Timestamp(0));
    start_time = NULL;
    finish_time = NULL;
}

DataThread::~DataThread() {
    if(start_time) delete start_time;
    if(finish_time) delete finish_time;
}

void DataThread::run() {
    this->setPriority(HighestPriority);
    data_receiver = data_source->spawn_receiver(this);
    connect(data_receiver, SIGNAL(started(Timestamp *)), SLOT(started(Timestamp *)));
    connect(data_receiver, SIGNAL(finished(Timestamp *)), SLOT(finished(Timestamp *)));
    snapshot_timer = new QTimer();
    connect(snapshot_timer, SIGNAL(timeout()), this, SLOT(create_new_snapshot()));
    exec();
    delete data_receiver;
    delete snapshot_timer;
}

void DataThread::register_observer(EventVisitor *visitor) {
    event_visitors.insert(visitor);
}

void DataThread::remove_observer(EventVisitor *visitor) {
    event_visitors.remove(visitor);
}

void DataThread::event_received(Event *event) {
    Snapshot *last = get_snapshot_list()->get_snapshot(current_snapshot->get_snapshot_id()-1);
    if(last->get_event_list()->get_event_list().size() > 1000) create_new_snapshot();
    last->add_event(event);
    event->apply_to(current_snapshot);
    foreach(EventVisitor *visitor, event_visitors) {
        event->accept(*visitor);
    }
}

void DataThread::create_new_snapshot() {
    current_snapshot = snapshot_list.append_snapshot();
}

void DataThread::started(Timestamp *time) {
    /* NOTE: get this from somewhere else . . . hardcoding it is a bad idea. */
    snapshot_timer->start(5000);
    start_time = time;
    emit data_started();
}

void DataThread::finished(Timestamp *time) {
    if(finish_time) return;
    snapshot_timer->stop();
    finish_time = time;
    emit data_finished();
}
