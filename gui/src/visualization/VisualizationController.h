#ifndef AESALON_GUI_VISUALIZATION_CONTROLLER_H
#define AESALON_GUI_VISUALIZATION_CONTROLLER_H

#include <QThread>

#include "VisualizationDataRange.h"
#include "VisualizationRenderer.h"
#include "session/DataThread.h"

class VisualizationController : public QThread { Q_OBJECT
private:
    DataThread *data_thread;
    VisualizationRenderer *renderer;
public:
    VisualizationController(DataThread *data_thread, VisualizationRenderer *renderer, QObject *parent);
    virtual ~VisualizationController();
    
protected:
    virtual void run();
public slots:
    void update_region(const VisualizationDataRange &range);
};

#endif
