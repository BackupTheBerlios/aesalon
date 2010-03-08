#ifndef AESALON_GUI_SESSION_VISUALIZATION_DENSITY_DATA_H
#define AESALON_GUI_SESSION_VISUALIZATION_DENSITY_DATA_H

#include "../VisualizationData.h"
#include "storage/MemoryTypes.h"

class DensityData : public VisualizationData {
private:
    Timestamp allocated_at, released_at;
    MemoryAddress address;
    MemorySize size;
    bool complete;
public:
    DensityData(const Timestamp &allocated_at, const Timestamp &released_at, MemoryAddress address, MemorySize size, bool complete);
    virtual ~DensityData();
    
    void set_release_time(const Timestamp &time);
    void set_complete() { complete = true; }
    
    virtual bool is_cachable() const;
    virtual void paint_onto(VisualizationRenderer* renderer) const;
};

#endif
