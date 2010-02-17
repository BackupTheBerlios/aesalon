#include "VisualizationThread.h"
#include "VisualizationThread.moc"

VisualizationThread::VisualizationThread(QObject *parent) : QThread(parent) {
    request_queue = new DataRequestQueue();
}

VisualizationThread::~VisualizationThread() {
    delete request_queue;
}

void VisualizationThread::run() {
    
}
