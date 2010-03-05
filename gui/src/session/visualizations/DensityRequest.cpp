#include "DensityRequest.h"

DensityRequest::DensityRequest(VisualizationThread *v_thread, const Timestamp &from, const Timestamp &to)
    : DataRequest(v_thread), from(from), to(to) {
}

DensityRequest::~DensityRequest() {

}

VisualizationData* DensityRequest::create_data() {
    
}

void DensityRequest::gather_data(DataThread *data_thread) {

}
