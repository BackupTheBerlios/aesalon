#include <QStack>
#include "ActiveBlocksRequest.h"
#include "ActiveBlocksData.h"
#include "../DataThread.h"
#include "storage/AllocEvent.h"
#include "storage/FreeEvent.h"

ActiveBlocksRequest::ActiveBlocksRequest(VisualizationThread* v_thread, const Timestamp &from, const Timestamp &to)
    : DataRequest(v_thread), from(from), to(to) {
}

ActiveBlocksRequest::~ActiveBlocksRequest() {
    
}

void ActiveBlocksRequest::visit(AllocEvent* event) {
    block_count ++;
    data_list.append(new ActiveBlocksData(event->get_timestamp(), block_count));
}

void ActiveBlocksRequest::visit(FreeEvent *event) {
    block_count --;
    data_list.append(new ActiveBlocksData(event->get_timestamp(), block_count));
}

QList<VisualizationData*> ActiveBlocksRequest::create_data() {
    return data_list;
}

void ActiveBlocksRequest::gather_data(DataThread* data_thread) {
    Snapshot *starting_snapshot = data_thread->get_snapshot_list()->get_snapshot_for(from); 
    block_count = starting_snapshot->get_block_count();
    data_list.append(new ActiveBlocksData(from, block_count));
    data_thread->get_snapshot_list()->iterate_through(from, to, *this);
}

