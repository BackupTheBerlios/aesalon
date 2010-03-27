#ifndef AESALON_GUI_VISUALIZATION_SINGLE_UPDATER_H
#define AESALON_GUI_VISUALIZATION_SINGLE_UPDATER_H

#include <QTimer>
#include "VisualizationUpdater.h"

class SingleUpdater : public VisualizationUpdater { Q_OBJECT
private:
    VisualizationDataRange range;
    QTimer *timer;
public:
    SingleUpdater(VisualizationDataRange *display_range, const VisualizationDataRange &range);
    virtual ~SingleUpdater();
private slots:
    void send_update();
};

#endif
