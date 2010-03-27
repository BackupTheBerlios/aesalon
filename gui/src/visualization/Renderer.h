#ifndef AESALON_GUI_VISUALIZATION_RENDERER_H
#define AESALON_GUI_VISUALIZATION_RENDERER_H

#include <QObject>

#include "Canvas.h"
#include "storage/Snapshot.h"

class Renderer : public QObject { Q_OBJECT
private:
    Canvas *current_canvas;
public:
    Renderer();
    virtual ~Renderer();
    
    void begin_rendering(Snapshot *snapshot);
    Canvas *end_rendering();
protected:
    virtual void initialize_rendering() = 0;
    virtual void finish_rendering() = 0;
};

#endif
