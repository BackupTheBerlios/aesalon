#include "DensityVisualization.h"
#include "DensityThread.h"

DensityVisualization::DensityVisualization(DataThread *data_thread, QWidget *parent): Visualization(data_thread, parent) {
    
}

DensityVisualization::~DensityVisualization() {

}

VisualizationThread* DensityVisualization::create_v_thread(DataThread *data_thread, QSize *canvas_size) {
    return new DensityThread(data_thread, canvas_size);
}
