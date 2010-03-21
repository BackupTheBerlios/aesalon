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
    
    @file session/SessionOverview.h
*/

#ifndef AESALON_GUI_SESSION_OVERVIEW_H
#define AESALON_GUI_SESSION_OVERVIEW_H

#include <QWidget>
#include <QLabel>
#include <QFormLayout>
#include <QPushButton>
#include "storage/Timestamp.h"
#include "VisualizationList.h"

class SessionOverview : public QWidget { Q_OBJECT
private:
    enum {
        WAITING,
        RUNNING,
        FINISHED
    } status;
    Timestamp start_timestamp;
    Timestamp finish_timestamp;
    DataThread *data_thread;
    
    QFormLayout *main_layout;
    QLabel *time_running_label;
    QPushButton *new_visualization_button;
    VisualizationList *v_list;
public:
    SessionOverview(DataThread *data_thread);
    ~SessionOverview();
private slots:
    void update_time_running();
    void new_visualization();
public slots:
    void started();
    void finished();
signals:
    void new_visualization(Visualization *visualization);
};

#endif
