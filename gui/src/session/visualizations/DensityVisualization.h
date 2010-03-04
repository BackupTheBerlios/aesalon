#ifndef AESALON_GUI_SESSION_VISUALIZATION_DENSITY_H
#define AESALON_GUI_SESSION_VISUALIZATION_DENSITY_H

#include "../Visualization.h"

class DensityVisualization : public Visualization {
public:
    DensityVisualization(DataThread* data_thread, QWidget* parent = 0);
    virtual ~DensityVisualization();
protected:
    virtual VisualizationThread* create_v_thread(DataThread *data_thread, QSize *canvas_size);
};

#endif
