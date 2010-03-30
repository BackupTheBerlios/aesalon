#ifndef AESALON_GUI_VISUALIZATION_BLOCK_COUNT_FACTORY_H
#define AESALON_GUI_VISUALIZATION_BLOCK_COUNT_FACTORY_H

#include "VisualizationFactory.h"

class BlockCountFactory : public VisualizationFactory {
public:
    virtual AxisFormatter *create_formatter() const;
    virtual Renderer *create_renderer() const;
};

#endif
