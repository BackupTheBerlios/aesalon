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
    while(true) {
        while(get_request_queue()->current_requests() > 0) {
            DataRequest *request = get_request_queue()->pop_request();
            VisualizationData *data = request->create_data();
            v_data.append(data);
        }
        msleep(50);
    }
}

void VisualizationThread::send_request(DataRequest *request) {
    data_thread->get_request_queue()->push_request(request);
}

void VisualizationThread::render(const Timestamp &from, const Timestamp &to) {
    
}
