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
    
    @file session/VisualizationThread.cpp
*/

#include <QColor>
#include "VisualizationThread.h"
#include "VisualizationThread.moc"
#include "VisualizationData.h"
#include "DataThread.h"

VisualizationThread::VisualizationThread(DataThread *data_thread, QObject *parent)
    : QThread(parent), data_thread(data_thread) {
    request_queue = new DataRequestQueue();
    current_request = NULL;
}

VisualizationThread::~VisualizationThread() {
    delete request_queue;
}

void VisualizationThread::run() {
    queue_timer = new QTimer();
    connect(queue_timer, SIGNAL(timeout()), SLOT(process_queue()));
    queue_timer->start(250);
    exec();
    delete queue_timer;
}

void VisualizationThread::send_request(DataRequest *request) {
    data_thread->get_request_queue()->push_request(request);
}

void VisualizationThread::update_request(VisualizationRequest *new_request) {
    current_request = new_request;
    get_request_queue()->clear_queue();
    current_request->set_renderer(new VisualizationRenderer(new_request->get_canvas(), is_splittable()));
    emit replace_image(current_image);
    generate_requests(current_request);
}

void VisualizationThread::render_request() {
    if(current_request) current_request->get_renderer()->update();
}

void VisualizationThread::process_queue() {
    bool changed = false;
    while(get_request_queue()->current_requests() > 0) {
        DataRequest *request = get_request_queue()->pop_request();
        if(request == NULL) {
            this->quit();
            return;
        }
        QList<VisualizationData *> data = request->create_data();
        v_data.append(data);
        if(current_request) {
            foreach(VisualizationData *vdata, data) {
                /* then add the data to the current renderer instance . . . */
                current_request->get_renderer()->add_data(vdata);
            }
            changed = true;
        }
    }
    
    if(changed && current_request) {
        current_request->get_renderer()->update();
        emit image_updated();
    }
}
