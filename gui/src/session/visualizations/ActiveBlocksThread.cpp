#include "ActiveBlocksThread.h"
#include "ActiveBlocksRequest.h"

ActiveBlocksThread::ActiveBlocksThread(DataThread *data_thread) : VisualizationThread(data_thread) {

}

ActiveBlocksThread::~ActiveBlocksThread() {

}

void ActiveBlocksThread::generate_requests(VisualizationRequest* current_request) {
    /* Generate one hundred data points for now. */
    /* TODO: make this configurable . . . */
    qint64 difference = current_request->get_from().ms_until(current_request->get_to());
    for(int x = 0; x < 100; x ++) {
        Timestamp time = current_request->get_from();
        time.add_ms(x * (difference / 100));
        send_request(new ActiveBlocksRequest(this, time));
    }
}
