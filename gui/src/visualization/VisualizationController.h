#ifndef AESALON_GUI_VISUALIZATION_CONTROLLER_H
#define AESALON_GUI_VISUALIZATION_CONTROLLER_H

#include <QThread>

#include "VisualizationDataRange.h"
#include "VisualizationRenderer.h"
#include "VisualizationUpdater.h"
#include "session/DataThread.h"

class VisualizationController : public QThread { Q_OBJECT
private:
    DataThread *data_thread;
    VisualizationRenderer *renderer;
    VisualizationUpdater *updater;
public:
    VisualizationController(DataThread *data_thread, VisualizationRenderer *renderer, QObject *parent);
    virtual ~VisualizationController();
protected:
    virtual void run();
public slots:
    void change_updater(VisualizationUpdater *new_updater);

    void update_region(const VisualizationDataRange &region);
    void update_region(const Timestamp &start, const Timestamp &end);
    void move_display(const Timestamp &start, const Timestamp &end);
signals:
    void move_display(const VisualizationDataRange &new_range);
};

#endif
