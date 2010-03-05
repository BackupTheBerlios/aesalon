#ifndef AESALON_GUI_SESSION_VISUALIZATION_DENSITY_DATA_H
#define AESALON_GUI_SESSION_VISUALIZATION_DENSITY_DATA_H

#include "../DataRequest.h"
#include "storage/Timestamp.h"
#include "storage/MemoryTypes.h"

class DensityRequest : public DataRequest {
private:
    Timestamp from, to;
    class BlockInfo {
    public:
        MemoryAddress address;
        MemorySize size;
        Timestamp allocation;
        Timestamp release;
    };
public:
    DensityRequest(VisualizationThread* v_thread, const Timestamp &from, const Timestamp &to);
    virtual ~DensityRequest();
    virtual QList<VisualizationData*> create_data();
    virtual void gather_data(DataThread* data_thread);
};

#endif
