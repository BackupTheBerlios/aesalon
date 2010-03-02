#include "ActiveBlocksThread.h"
#include "ActiveBlocksRequest.h"
#include "../DataThread.h"

ActiveBlocksThread::ActiveBlocksThread(DataThread *data_thread) : VisualizationThread(data_thread) {

}

ActiveBlocksThread::~ActiveBlocksThread() {

}

void ActiveBlocksThread::generate_requests(VisualizationRequest* current_request) {
    qDebug("ActiveBlocksThread::generate_requests() called . . .");
    /* TODO: make this configurable . . . */
    int data_points = 100;
    qint64 difference = current_request->get_from().ms_until(current_request->get_to());
    for(int x = 0; x < data_points; x ++) {
        Timestamp time = current_request->get_from();
        time.add_ms(x * (difference / qreal(data_points)));
        send_request(new ActiveBlocksRequest(this, time));
    }
    qDebug("DataThread request queue size: %i", get_data_thread()->get_request_queue()->current_requests());
}

