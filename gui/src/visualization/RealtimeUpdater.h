#ifndef AESALON_GUI_VISUALIZATION_REALTIME_UPDATER_H
#define AESALON_GUI_VISUALIZATION_REALTIME_UPDATER_H

#include "VisualizationUpdater.h"

class RealtimeUpdater : public VisualizationUpdater { Q_OBJECT
public:
    RealtimeUpdater();
    virtual ~RealtimeUpdater();
};

#endif
