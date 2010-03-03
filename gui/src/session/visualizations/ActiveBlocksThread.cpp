#include "ActiveBlocksThread.h"
#include "ActiveBlocksRequest.h"
#include "../DataThread.h"

ActiveBlocksThread::ActiveBlocksThread(DataThread *data_thread, QSize *canvas_size) : VisualizationThread(data_thread, canvas_size) {

}

ActiveBlocksThread::~ActiveBlocksThread() {

}

void ActiveBlocksThread::generate_requests(VisualizationRequest* current_request) {
    /* TODO: make this configurable . . . */
    int data_points = 100;
    qint64 difference = current_request->get_from().ms_until(current_request->get_to());
    for(int x = 0; x < data_points; x ++) {
        Timestamp time = current_request->get_from();
        time.add_ms(x * (difference / qreal(data_points)));
        send_request(new ActiveBlocksRequest(this, time));
    }
}

