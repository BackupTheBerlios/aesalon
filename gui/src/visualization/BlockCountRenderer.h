#ifndef AESALON_GUI_VISUALIZATION_BLOCK_COUNT_RENDERER_H
#define AESALON_GUI_VISUALIZATION_BLOCK_COUNT_RENDERER_H

#include "Renderer.h"

class BlockCountRenderer : public Renderer {
private:
    qreal block_count;
    DataPoint point;
public:
    BlockCountRenderer(Canvas *canvas);
    virtual ~BlockCountRenderer();
protected:
    virtual void visit(AllocEvent *event);
    virtual void visit(FreeEvent *event);
};

#endif
