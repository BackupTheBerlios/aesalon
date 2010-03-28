#ifndef AESALON_GUI_VISUALIZATION_DENSITY_RENDERER_H
#define AESALON_GUI_VISUALIZATION_DENSITY_RENDERER_H

#include <QMap>

#include "Renderer.h"

class DensityRenderer : public Renderer {
private:
    Canvas *canvas;
    QMap<qint64, DataRange> blocks;
public:
    DensityRenderer();
    virtual ~DensityRenderer();

    virtual void begin_rendering(const DataRange &render_range, Snapshot *snapshot);
    virtual Canvas* end_rendering();
    
    virtual void visit(AllocEvent *event);
    virtual void visit(FreeEvent *event);
private:
    void assemble_blocks(BiTreeNode *node);
};

#endif
