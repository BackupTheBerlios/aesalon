#include "ActiveBlocksRequest.h"
#include "storage/Snapshot.h"
#include "storage/SnapshotList.h"
#include "GraphDataEngine.h"
#include "GraphData.h"

ActiveBlocksRequest::ActiveBlocksRequest(GraphDataEngine *data_engine, const Timestamp &timestamp)
    : DataRequest(data_engine), timestamp(timestamp) {
}

ActiveBlocksRequest::~ActiveBlocksRequest() {

}

void ActiveBlocksRequest::construct_response(SnapshotList *snapshot_list) {
    Snapshot *snapshot = snapshot_list->get_snapshot_for(timestamp);
    count = count_blocks(snapshot->get_head_node());
}

void ActiveBlocksRequest::process_response() {
    get_requester()->add_data(new GraphData(timestamp, qreal(count)));
}


int ActiveBlocksRequest::count_blocks(BiTreeNode *node) const {
    int count = 0;
    if(node->get_left()) count += count_blocks(node->get_left());
    if(node->get_right()) count += count_blocks(node->get_right());
    count += node->get_block_list_size();
    
    return count;
}
