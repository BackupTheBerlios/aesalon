#ifndef AESALON_GUI_VISUALIZATION_UPDATER_H
#define AESALON_GUI_VISUALIZATION_UPDATER_H

#include <QObject>

#include "VisualizationDataRange.h"

class VisualizationUpdater : public QObject { Q_OBJECT
public:
    VisualizationUpdater();
    virtual ~VisualizationUpdater();
signals:
    void render_region(const VisualizationDataRange &region);
    void move_display(const VisualizationDataRange &region);
};

#endif
