#include "DensityThread.h"
#include "DensityRequest.h"

DensityThread::DensityThread(DataThread *data_thread, QObject *parent): VisualizationThread(data_thread, parent) {
    
}

DensityThread::~DensityThread() {

}

void DensityThread::generate_requests(VisualizationRequest *current_request) {
    send_request(new DensityRequest(this, current_request->get_from(), current_request->get_to()));
}
