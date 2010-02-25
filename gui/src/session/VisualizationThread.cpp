#include <QColor>
#include "VisualizationThread.h"
#include "VisualizationThread.moc"
#include "VisualizationData.h"
#include "DataThread.h"

VisualizationThread::VisualizationThread(DataThread *data_thread, QObject *parent) : QThread(parent), data_thread(data_thread) {
    
    request_queue = new DataRequestQueue();
}

VisualizationThread::~VisualizationThread() {
    delete request_queue;
}

void VisualizationThread::run() {
    bool finished = false;
    while(!finished) {
        while(get_request_queue()->current_requests() > 0) {
            DataRequest *request = get_request_queue()->pop_request();
            if(request == NULL) {
                finished = true;
                break;
            }
            VisualizationData *data = request->create_data();
            v_data.append(data);
            if(current_request && data->is_within(current_request)) {
                /* then draw the data. */
                data->render_onto(current_image, current_request);
            }
        }
        msleep(50);
    }
}

void VisualizationThread::send_request(DataRequest *request) {
    qDebug("VisualizationThread::send_request() called . . .");
    data_thread->get_request_queue()->push_request(request);
}

void VisualizationThread::update_request(VisualizationRequest *new_request) {
    current_request = new_request;
    current_image = new QImage(200, 100, QImage::Format_RGB32);
    current_image->fill(QColor(255, 255, 255).rgb());
    emit replace_image(current_image);
    generate_requests(current_request);
}
