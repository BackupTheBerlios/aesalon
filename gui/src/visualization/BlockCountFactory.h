#ifndef AESALON_GUI_VISUALIZATION_BLOCK_COUNT_FACTORY_H
#define AESALON_GUI_VISUALIZATION_BLOCK_COUNT_FACTORY_H

#include "VisualizationFactory.h"

class BlockCountFactory : public VisualizationFactory {
public:
    BlockCountFactory(DataThread* data_thread);

    virtual AxisFormatter *create_formatter() const;
    virtual Renderer *create_renderer(Canvas *canvas) const;
    
    virtual QString get_vertical_axis_label() const { return "Block count"; }
};

#endif
