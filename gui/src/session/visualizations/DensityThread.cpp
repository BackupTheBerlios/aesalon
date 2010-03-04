#include "DensityThread.h"

DensityThread::DensityThread(DataThread *data_thread, QSize *canvas_size, QObject *parent): VisualizationThread(data_thread, canvas_size, parent) {
    
}

DensityThread::~DensityThread() {

}

void DensityThread::generate_requests(VisualizationRequest *current_request) {
    
}
