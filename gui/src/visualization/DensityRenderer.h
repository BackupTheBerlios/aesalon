#ifndef AESALON_GUI_VISUALIZATION_DENSITY_RENDERER_H
#define AESALON_GUI_VISUALIZATION_DENSITY_RENDERER_H

#include <QMap>

#include "VisualizationRenderer.h"

class DensityRenderer : public VisualizationRenderer { Q_OBJECT
private:
    QMap<MemoryAddress, VisualizationDataRange> blocks;
public:
    DensityRenderer(QImage *image);
    virtual ~DensityRenderer();
    
    virtual void begin_update(Snapshot *starting_snaphot);
    virtual void end_update();
private:
    void gather_blocks(BiTreeNode *node);
protected:
    virtual void visit(AllocEvent *event);
    virtual void visit(FreeEvent *event);
};

#endif
