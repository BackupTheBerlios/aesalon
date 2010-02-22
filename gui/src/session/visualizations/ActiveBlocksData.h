#ifndef AESALON_GUI_SESSION_VISUALIZATION_ACTIVE_BLOCKS_DATA_H
#define AESALON_GUI_SESSION_VISUALIZATION_ACTIVE_BLOCKS_DATA_H

#include <QImage>

#include "../VisualizationData.h"

class ActiveBlocksData : public VisualizationData {
private:
    Timestamp timestamp;
    int value;
public:
    ActiveBlocksData(const Timestamp &timestamp, int value);
    virtual ~ActiveBlocksData();
    
    virtual bool render_onto(QImage *image, VisualizationRequest *request) const;
    
    virtual bool is_within(VisualizationRequest* request) const;
};

#endif
