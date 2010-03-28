#include "DensityFactory.h"
#include "DensityRenderer.h"
#include "DensityFormatter.h"

Renderer *DensityFactory::create_renderer() const {
    return new DensityRenderer();
}

AxisFormatter *DensityFactory::create_formatter() const {
    return new DensityFormatter();
}

