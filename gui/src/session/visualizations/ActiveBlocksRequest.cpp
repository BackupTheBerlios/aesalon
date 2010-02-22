#include <QStack>
#include "ActiveBlocksRequest.h"
#include "../DataThread.h"

ActiveBlocksRequest::ActiveBlocksRequest(VisualizationThread* v_thread, const Timestamp &timestamp)
    : DataRequest(v_thread), timestamp(timestamp) {
    
}

ActiveBlocksRequest::~ActiveBlocksRequest() {
    
}

VisualizationData* ActiveBlocksRequest::create_data() {
    return 0;
}

void ActiveBlocksRequest::gather_data(DataThread* data_thread) {
    SnapshotList *snapshot_list = data_thread->get_snapshot_list();
    Snapshot *snapshot = snapshot_list->get_snapshot_for(timestamp);
    /* NOTE: optimize this later . . . */
    BiTreeNode *node = snapshot->get_head_node();
    if(!node) {
        blocks = 0;
        return;
    }
    QStack<BiTreeNode *> node_stack;
    node_stack.push_back(node->get_left());
    node_stack.push_back(node->get_right());
    
    while(node_stack.size()) {
        node = node_stack.pop();
        if(node == NULL) continue;
        if(node->get_block_list_size()) blocks += node->get_block_list_size();
        else {
            node_stack.push_back(node->get_left());
            node_stack.push_back(node->get_right());
        }
    }
}
