#ifndef AESALON_GUI_VISUALIZATION_DENSITY_RENDERER_H
#define AESALON_GUI_VISUALIZATION_DENSITY_RENDERER_H

#include <QMap>

#include "Renderer.h"

class DensityRenderer : public Renderer {
private:
    QMap<qint64, CanvasObject *> blocks;
public:
    DensityRenderer(Canvas *canvas);
    virtual ~DensityRenderer();

    virtual void visit(AllocEvent *event);
    virtual void visit(FreeEvent *event);
};

#endif
