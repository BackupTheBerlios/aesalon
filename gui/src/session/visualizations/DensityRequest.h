#ifndef AESALON_GUI_SESSION_VISUALIZATION_DENSITY_REQUEST_H
#define AESALON_GUI_SESSION_VISUALIZATION_DENSITY_REQUEST_H

#include <QMap>

#include "../DataRequest.h"
#include "storage/Timestamp.h"
#include "storage/MemoryTypes.h"
#include "storage/EventVisitor.h"
#include "storage/Block.h"
#include "storage/BiTreeNode.h"
#include "DensityData.h"

class DensityRequest : public DataRequest, public EventVisitor {
private:
    Timestamp from, to;
    QList<VisualizationData *> data_list;
    
    QMap<MemoryAddress, DensityData *> density_data;
public:
    DensityRequest(VisualizationThread* v_thread, const Timestamp &from, const Timestamp &to);
    virtual ~DensityRequest();
    virtual QList<VisualizationData*> create_data();
    virtual void gather_data(DataThread* data_thread);
    
    virtual void visit(AllocEvent* event);
    virtual void visit(FreeEvent* event);
private:
    void gather_blocks(BiTreeNode *node = NULL);
};

#endif
