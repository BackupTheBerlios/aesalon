#ifndef AESALON_GUI_SESSION_VISUALIZATION_ACTIVE_BLOCKS_DATA_H
#define AESALON_GUI_SESSION_VISUALIZATION_ACTIVE_BLOCKS_DATA_H

#include <QPixmap>

#include "../VisualizationData.h"

class ActiveBlocksData : public VisualizationData {
private:
    Timestamp timestamp;
    int value;
public:
    ActiveBlocksData(const Timestamp &timestamp, int value);
    virtual ~ActiveBlocksData();
    
    virtual void paint_onto(VisualizationRenderer* renderer) const;
    
    virtual bool is_cachable() const { return true; }
};

#endif
