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
    
    @file session/DataThread.h
*/

#ifndef AESALON_GUI_SESSION_DATA_THREAD_H
#define AESALON_GUI_SESSION_DATA_THREAD_H

#include <QThread>
#include <QTimer>
#include <QQueue>

#include "storage/SnapshotList.h"
#include "data/DataSource.h"

class DataThread : public QThread { Q_OBJECT
private:
    DataSource *data_source;
    DataReceiver *data_receiver;
    
    SnapshotList snapshot_list;
    Snapshot *current_snapshot;
    QTimer *snapshot_timer;
    
    Timestamp *start_time, *finish_time;
public:
    DataThread(QObject *parent, DataSource *data_source);
    virtual ~DataThread();
    
    void event_received(Event *event);
    
    const Timestamp *get_start_time() const { return start_time; }
    const Timestamp *get_finish_time() const { return finish_time; }
    
    SnapshotList *get_snapshot_list() { return &snapshot_list; }
private slots:
    void create_new_snapshot();
    void started(Timestamp *time);
    void finished(Timestamp *time);
protected:
    virtual void run();
signals:
    void data_started();
    void data_finished();
};

#endif
