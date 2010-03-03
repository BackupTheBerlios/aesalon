#ifndef AESALON_GUI_SESSION_VISUALIZATION_ACTIVE_BLOCKS_H
#define AESALON_GUI_SESSION_VISUALIZATION_ACTIVE_BLOCKS_H

#include "../Visualization.h"

class ActiveBlocksVisualization : public Visualization { Q_OBJECT
protected:
    virtual VisualizationThread* create_v_thread(DataThread* data_thread, QSize* canvas_size);
public:
    ActiveBlocksVisualization(DataThread *data_thread);
    virtual ~ActiveBlocksVisualization();
    
    static QString get_static_title() {
        return tr("Active Blocks");
    }
    virtual QString get_title() const {
        return tr("Active Blocks");
    }
    
};

#endif
