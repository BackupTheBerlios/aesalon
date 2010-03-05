#include "DensityRequest.h"

DensityRequest::DensityRequest(VisualizationThread *v_thread, const Timestamp &from, const Timestamp &to)
    : DataRequest(v_thread), from(from), to(to) {
}

DensityRequest::~DensityRequest() {

}

QList<VisualizationData *> DensityRequest::create_data() {
    return QList<VisualizationData *>();
}

void DensityRequest::gather_data(DataThread *data_thread) {

}
