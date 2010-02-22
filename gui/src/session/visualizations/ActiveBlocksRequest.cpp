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
    blocks = count_blocks(snapshot->get_head_node());
    qDebug("ActiveBlocksRequest: found %i blocks for %s . . .", blocks, timestamp.to_string().toStdString().c_str());
#if 0
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
#endif
    
}

int ActiveBlocksRequest::count_blocks(BiTreeNode* node) {
    if(node == NULL) return 0;
    int count = node->get_block_list_size();
    if(count) {
        qDebug("count_blocks(): found %i block(s) (snapshot ID is %lli).", count, node->get_snapshot_id());
    }
    if(node->get_left()) count += count_blocks(node->get_left());
    if(node->get_right()) count += count_blocks(node->get_right());
    return count;
}
