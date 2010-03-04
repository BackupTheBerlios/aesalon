#include <QColor>
#include "VisualizationThread.h"
#include "VisualizationThread.moc"
#include "VisualizationData.h"
#include "DataThread.h"

VisualizationThread::VisualizationThread(DataThread *data_thread, QSize *canvas_size, QObject *parent)
    : QThread(parent), data_thread(data_thread), canvas_size(canvas_size) {
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
    current_image = new QPixmap(200, 100);
    current_image->fill(QColor(255, 255, 255).rgb());
    current_request->set_renderer(new VisualizationRenderer(current_image, is_splittable()));
    emit replace_image(current_image);
    generate_requests(current_request);
}

void VisualizationThread::update_graph() {
    if(current_request) current_request->get_renderer()->update(*canvas_size);
}

void VisualizationThread::process_queue() {
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
        }
    }
    if(current_request && canvas_size) {
        current_request->get_renderer()->update(*canvas_size);
        emit image_updated();
    }
}
