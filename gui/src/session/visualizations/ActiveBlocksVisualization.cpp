#include "ActiveBlocksVisualization.h"
#include "ActiveBlocksVisualization.moc"
#include "ActiveBlocksThread.h"

ActiveBlocksVisualization::ActiveBlocksVisualization(DataThread* data_thread): Visualization(data_thread) {
}

ActiveBlocksVisualization::~ActiveBlocksVisualization() {
}

VisualizationThread* ActiveBlocksVisualization::create_v_thread(DataThread *data_thread, QSize *canvas_size) {
    return new ActiveBlocksThread(data_thread, canvas_size);
}
