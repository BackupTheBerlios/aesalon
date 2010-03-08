#ifndef AESALON_GUI_SESSION_VISUALIZATION_ACTIVE_BLOCKS_REQUEST_H
#define AESALON_GUI_SESSION_VISUALIZATION_ACTIVE_BLOCKS_REQUEST_H

#include "../DataRequest.h"
#include "storage/Timestamp.h"
#include "storage/BiTreeNode.h"
#include "ActiveBlocksData.h"
#include "storage/EventVisitor.h"

class ActiveBlocksRequest : public DataRequest, public EventVisitor {
private:
    Timestamp from, to;
    QList<VisualizationData *> data_list;
    quint64 block_count;
public:
    ActiveBlocksRequest(VisualizationThread* v_thread, const Timestamp &from, const Timestamp &to);
    virtual ~ActiveBlocksRequest();
    
    virtual void visit(AllocEvent *event);
    virtual void visit(FreeEvent *event);
    
    virtual QList<VisualizationData*> create_data();
    virtual void gather_data(DataThread* data_thread);
};

#endif
