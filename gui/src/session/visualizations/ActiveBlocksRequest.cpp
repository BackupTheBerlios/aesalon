#include <QStack>
#include "ActiveBlocksRequest.h"
#include "ActiveBlocksData.h"
#include "../DataThread.h"

ActiveBlocksRequest::ActiveBlocksRequest(VisualizationThread* v_thread, const Timestamp &timestamp)
    : DataRequest(v_thread), timestamp(timestamp) {
}

ActiveBlocksRequest::~ActiveBlocksRequest() {
    
}

VisualizationData* ActiveBlocksRequest::create_data() {
    return new ActiveBlocksData(timestamp, blocks);
}

void ActiveBlocksRequest::gather_data(DataThread* data_thread) {
    SnapshotList *snapshot_list = data_thread->get_snapshot_list();
    Snapshot *snapshot = snapshot_list->get_snapshot_for(timestamp);
    if(snapshot == NULL) {
        qDebug("NULL snapshot!");
        blocks = 0;
        return;
    }
    /* NOTE: optimize this later . . . */
    BiTreeNode *node = snapshot->get_head_node();
    if(!node) {
        blocks = 0;
        return;
    }
    QStack<BiTreeNode *> node_stack;
    node_stack.push_back(node->get_left());
    node_stack.push_back(node->get_right());
    
    blocks = 0;
    
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
