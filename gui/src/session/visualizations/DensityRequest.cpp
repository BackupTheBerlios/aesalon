#include "DensityRequest.h"
#include "DensityData.h"
#include "storage/AllocEvent.h"
#include "storage/FreeEvent.h"
#include "../DataThread.h"
#include "../VisualizationData.h"

QList<QRgb> DensityRequest::colours;

DensityRequest::DensityRequest(VisualizationThread *v_thread, const Timestamp &from, const Timestamp &to)
    : DataRequest(v_thread), from(from), to(to) {
    if(colours.size() == 0) {
        colours.append(qRgb(96, 0, 0));
        colours.append(qRgb(0, 96, 0));
        colours.append(qRgb(0, 0, 96));
        colours.append(qRgb(96, 96, 0));
        colours.append(qRgb(96, 0, 96));
        colours.append(qRgb(0, 96, 96));
        colours.append(qRgb(96, 96, 96));
    }
}

DensityRequest::~DensityRequest() {

}

QRgb DensityRequest::get_colour() {
    QRgb col = colours[colour_index];
    colour_index ++;
    colour_index %= colours.size();
    return col;
}


QList<VisualizationData *> DensityRequest::create_data() {
    return data_list;
}

void DensityRequest::gather_data(DataThread *data_thread) {
    colour_index = 0;
    Snapshot *starting_snapshot = data_thread->get_snapshot_list()->get_snapshot_for(from); 
    gather_blocks(starting_snapshot->get_head_node());
    data_thread->get_snapshot_list()->iterate_through(from, to, *this);
    
    foreach(DensityData *data, density_data) {
        data->set_release_time(to);
        data_list.append(data);
    }
}

void DensityRequest::visit(AllocEvent *event) {
    density_data[event->get_address()] =
        new DensityData(event->get_timestamp(), Timestamp(0), event->get_address(), event->get_size(), false);
    density_data[event->get_address()]->set_colour(get_colour());
}

void DensityRequest::visit(FreeEvent *event) {
    DensityData *data = density_data[event->get_address()];
    data->set_release_time(event->get_timestamp());
    data->set_complete();
    data_list.append(data);
    density_data.remove(event->get_address());
}

void DensityRequest::gather_blocks(BiTreeNode *node) {
    if(node->get_left()) gather_blocks(node->get_left());
    if(node->get_right()) gather_blocks(node->get_right());
    if(node->get_block_list_size()) {
        foreach(Block *block, node->get_block_list()) {
            density_data[block->get_address()] =
                new DensityData(block->get_allocation_time(), block->get_release_time(), block->get_address(), block->get_size(),
                    block->get_release_time() != Timestamp(0));
            density_data[block->get_address()]->set_colour(get_colour());
        }
    }
}
