#include "CanvasGeneratorThread.h"
#include "CanvasGeneratorThread.moc"

CanvasGeneratorThread::CanvasGeneratorThread(DataThread *data_thread, Renderer *renderer)
    : QThread(), data_thread(data_thread), renderer(renderer) {
    
}

CanvasGeneratorThread::~CanvasGeneratorThread() {

}

void CanvasGeneratorThread::run() {
    data_thread->get_snapshot_list()->iterate_through(Timestamp(0), Timestamp::NOW, *renderer);
}
