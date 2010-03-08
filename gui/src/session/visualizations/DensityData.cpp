#include "DensityData.h"
#include "../VisualizationRenderer.h"

DensityData::DensityData(const Timestamp &allocated_at, const Timestamp &released_at, MemoryAddress address, MemorySize size, bool complete)
    : allocated_at(allocated_at), released_at(released_at), address(address), size(size), complete(complete) {
    get_data_range().set_lower_time(allocated_at);
    get_data_range().set_upper_time(released_at);
    get_data_range().set_lower_data(address);
    get_data_range().set_upper_data(address+size);
    /*qDebug("DensityData(): address is %lli, size is %lli", address, size);*/
    colour = qRgb(0, 64, 32);
}

DensityData::~DensityData() {

}

void DensityData::set_release_time(const Timestamp& time) {
    released_at = time;
    get_data_range().set_upper_time(released_at);
}

bool DensityData::is_cachable() const {
    return complete;
}

void DensityData::paint_onto(VisualizationRenderer* renderer) const {
    VisualizationRenderPoint from(0, 0), to(0, 0);
    from.set_data_element(address);
    to.set_data_element(address + size);
    from.set_time_element(allocated_at);
    to.set_time_element(released_at);
    renderer->paint_box(from, to, qRgb(0, 0, 0), colour, Qt::SolidLine, Qt::SolidPattern);
}
