#ifndef AESALON_GUI_SESSION_VISUALIZATION_DENSITY_H
#define AESALON_GUI_SESSION_VISUALIZATION_DENSITY_H

#include "../Visualization.h"

class DensityVisualization : public Visualization {
public:
    DensityVisualization(DataThread* data_thread, QWidget* parent = 0);
    virtual ~DensityVisualization();
    
    static QString get_static_title() {
        return "Block Density";
    }
    virtual QString get_title() const {
        return "Block Density";
    }
protected:
    virtual VisualizationThread* create_v_thread(DataThread *data_thread);
};

#endif
