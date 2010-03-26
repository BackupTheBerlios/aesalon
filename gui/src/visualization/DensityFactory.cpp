#include "DensityFactory.h"
#include "DensityDisplay.h"
#include "DensityRenderer.h"

DensityFactory::DensityFactory() {

}

DensityFactory::~DensityFactory() {

}

VisualizationDisplay *DensityFactory::create_display(QWidget *parent) const {
    return new DensityDisplay(parent);
}

VisualizationRenderer *DensityFactory::create_renderer(QImage *image) const {
    return new DensityRenderer(image);
}
