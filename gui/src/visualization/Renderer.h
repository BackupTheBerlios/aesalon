#ifndef AESALON_GUI_VISUALIZATION_RENDERER_H
#define AESALON_GUI_VISUALIZATION_RENDERER_H

#include <QObject>

#include "Canvas.h"
#include "storage/Snapshot.h"
#include "storage/EventVisitor.h"

class Renderer : public EventVisitor {
protected:
    Canvas *canvas;
    SnapshotID last_snapshot;
public:
    Renderer(Canvas *canvas);
    virtual ~Renderer();
    
    Canvas *get_canvas() const { return canvas; }
    
    SnapshotID get_last_snapshot() const { return last_snapshot; }
    void update_last_snapshot(SnapshotID snapshot) { last_snapshot = snapshot; }
};

#endif
