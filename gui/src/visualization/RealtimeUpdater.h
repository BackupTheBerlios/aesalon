#ifndef AESALON_GUI_VISUALIZATION_REALTIME_UPDATER_H
#define AESALON_GUI_VISUALIZATION_REALTIME_UPDATER_H

#include <QTimer>
#include "VisualizationUpdater.h"

class RealtimeUpdater : public VisualizationUpdater { Q_OBJECT
private:
    int history_length, cycle_length;
    QTimer *cycle_timer;
    VisualizationDataRange range;
public:
    RealtimeUpdater(VisualizationDataRange *display_range, int history_length, int cycle_length);
    virtual ~RealtimeUpdater();
private slots:
    void render_cycle();
};

#endif
