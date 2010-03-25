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
    
    @file session/VisualizationThread.h
*/

#ifndef AESALON_GUI_SESSION_VISUALIZATION_THREAD_H
#define AESALON_GUI_SESSION_VISUALIZATION_THREAD_H

#include <QList>
#include <QThread>
#include <QPixmap>
#include <QTimer>

#include "VisualizationRenderer.h"
#include "DataRequestQueue.h"
#include "storage/Timestamp.h"

class VisualizationThread : public QThread { Q_OBJECT
private:
    DataThread *data_thread;
    DataRequestQueue *request_queue;
    QList<VisualizationData *> v_data;
    QPixmap *current_image;
    QTimer *queue_timer;
public:
    VisualizationThread(DataThread *data_thread, QObject *parent = 0);
    virtual ~VisualizationThread();
    
    DataRequestQueue *get_request_queue() const { return request_queue; }
protected:
    virtual void run();
    void send_request(DataRequest *request);
    DataThread *get_data_thread() const { return data_thread; }
    virtual bool is_splittable() const = 0;
private slots:
    void process_queue();
signals:
    void replace_image(QPixmap *new_image);
    void image_updated();
};

#endif
