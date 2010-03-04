#ifndef AESALON_GUI_SESSION_VISUALIZATION_DENSITY_THREAD_H
#define AESALON_GUI_SESSION_VISUALIZATION_DENSITY_THREAD_H

#include "../VisualizationThread.h"

class DensityThread : public VisualizationThread {
protected:
    virtual void generate_requests(VisualizationRequest* current_request);
    virtual bool is_splittable() const { return false; }
public:
    DensityThread(DataThread* data_thread, QSize* canvas_size, QObject* parent = 0);
    virtual ~DensityThread();
};

#endif
