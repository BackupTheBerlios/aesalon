#ifndef AESALON_GUI_SESSION_VISUALIZATION_ACTIVE_BLOCKS_REQUEST_H
#define AESALON_GUI_SESSION_VISUALIZATION_ACTIVE_BLOCKS_REQUEST_H

#include "../DataRequest.h"
#include "storage/Timestamp.h"
#include "storage/BiTreeNode.h"
#include "ActiveBlocksData.h"

class ActiveBlocksRequest : public DataRequest {
private:
    Timestamp from, to;
    QList<ActiveBlocksData *> data_list;
public:
    ActiveBlocksRequest(VisualizationThread* v_thread, const Timestamp &from, const Timestamp &to);
    virtual ~ActiveBlocksRequest();
    
    virtual QList<VisualizationData*> create_data();
    virtual void gather_data(DataThread* data_thread);
};

#endif
