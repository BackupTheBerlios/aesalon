#ifndef AESALON_GUI_SESSION_VISUALIZATION_LIST_H
#define AESALON_GUI_SESSION_VISUALIZATION_LIST_H

#include <QListWidget>
#include "Visualization.h"

class VisualizationList : public QListWidget { Q_OBJECT
public:
    VisualizationList();
    virtual ~VisualizationList();
    
    Visualization *create_from_selected(DataThread *data_thread) const;
};

#endif
