#ifndef AESALON_GUI_VISUALIZATION_RENDERER_H
#define AESALON_GUI_VISUALIZATION_RENDERER_H

#include <QObject>

#include "Canvas.h"
#include "storage/Snapshot.h"
#include "storage/EventVisitor.h"

class Renderer : public EventVisitor {
public:
    Renderer();
    virtual ~Renderer();
    
    virtual void begin_rendering(const DataRange &render_range, Snapshot *snapshot) = 0;
    virtual Canvas *end_rendering() = 0;
};

#endif
