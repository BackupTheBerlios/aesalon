#ifndef AESALON_GUI_SESSION_ACTIVE_BLOCKS_REQUEST_H
#define AESALON_GUI_SESSION_ACTIVE_BLOCKS_REQUEST_H

#include "DataRequest.h"
#include "storage/Timestamp.h"
#include "storage/BiTreeNode.h"

class ActiveBlocksRequest : public DataRequest {
private:
    Timestamp timestamp;
    int count;
public:
    ActiveBlocksRequest(GraphDataEngine *data_engine, Timestamp timestamp);
    virtual ~ActiveBlocksRequest();
    
    virtual void construct_response(SnapshotList *snapshot_list);
    virtual void process_response();
private:
    int count_blocks(BiTreeNode *node) const;
};

#endif
