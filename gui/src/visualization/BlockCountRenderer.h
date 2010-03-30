#ifndef AESALON_GUI_VISUALIZATION_BLOCK_COUNT_RENDERER_H
#define AESALON_GUI_VISUALIZATION_BLOCK_COUNT_RENDERER_H

#include "Renderer.h"

class BlockCountRenderer : public Renderer {
private:
    Canvas *canvas;
public:
    BlockCountRenderer();
    virtual ~BlockCountRenderer();
    virtual void begin_rendering(const DataRange &render_range, Snapshot *snapshot);
    virtual Canvas *end_rendering();
    qreal block_count;
    DataPoint point;
protected:
    virtual void visit(AllocEvent *event);
    virtual void visit(FreeEvent *event);
private:
    void count_blocks(BiTreeNode *node);
};

#endif
