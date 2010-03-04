#include "ActiveBlocksThread.h"
#include "ActiveBlocksRequest.h"
#include "../DataThread.h"

ActiveBlocksThread::ActiveBlocksThread(DataThread *data_thread, QSize *canvas_size) : VisualizationThread(data_thread, canvas_size) {

}

ActiveBlocksThread::~ActiveBlocksThread() {

}

void ActiveBlocksThread::generate_requests(VisualizationRequest* current_request) {
    send_request(new ActiveBlocksRequest(this, current_request->get_from(), current_request->get_to()));
}

