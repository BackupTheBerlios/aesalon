#ifndef AESALON_GUI_SESSION_VISUALIZATION_ACTIVE_BLOCKS_REQUEST_H
#define AESALON_GUI_SESSION_VISUALIZATION_ACTIVE_BLOCKS_REQUEST_H

#include "../DataRequest.h"
#include "storage/Timestamp.h"
#include "storage/BiTreeNode.h"

class ActiveBlocksRequest : public DataRequest {
private:
    Timestamp timestamp;
    int blocks;
public:
    ActiveBlocksRequest(VisualizationThread* v_thread, const Timestamp &timestamp);
    virtual ~ActiveBlocksRequest();
    
    virtual VisualizationData* create_data();
    virtual void gather_data(DataThread* data_thread);
private:
    int count_blocks(BiTreeNode *node);
};

#endif
