#ifndef AESALON_GUI_VISUALIZATION_UPDATER_H
#define AESALON_GUI_VISUALIZATION_UPDATER_H

#include <QObject>

#include "VisualizationDataRange.h"

class VisualizationUpdater : public QObject { Q_OBJECT
protected:
    VisualizationDataRange *display_range;
public:
    VisualizationUpdater(VisualizationDataRange *display_range);
    virtual ~VisualizationUpdater();
signals:
    void render_region(const VisualizationDataRange &region);
    void render_region(const Timestamp &from, const Timestamp &to);
    void move_display(const VisualizationDataRange &region);
    void move_display(const Timestamp &begin, const Timestamp &end);
};

#endif
