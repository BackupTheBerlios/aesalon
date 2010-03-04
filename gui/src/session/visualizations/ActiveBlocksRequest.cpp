#include <QStack>
#include "ActiveBlocksRequest.h"
#include "ActiveBlocksData.h"
#include "../DataThread.h"

ActiveBlocksRequest::ActiveBlocksRequest(VisualizationThread* v_thread, const Timestamp &from, const Timestamp &to)
    : DataRequest(v_thread), from(from), to(to) {
}

ActiveBlocksRequest::~ActiveBlocksRequest() {
    
}

QList<VisualizationData*> ActiveBlocksRequest::create_data() {
    QList<VisualizationData*> list;
    foreach(ActiveBlocksData *data, data_list) {
        list.append(data);
    }
    return list;
}

void ActiveBlocksRequest::gather_data(DataThread* data_thread) {
    SnapshotList *snapshot_list = data_thread->get_snapshot_list();
    Snapshot *snapshot = snapshot_list->get_snapshot_for(from);
    if(snapshot == NULL) {
        return;
    }
    while(snapshot->get_timestamp() < to) {
        data_list.append(new ActiveBlocksData(snapshot->get_timestamp(), snapshot->get_block_count()));
        if(!snapshot_list->move_snapshot_to_next_event(snapshot)) break;
    }
    
    snapshot->free_tree();    
}
