#include "DensityFactory.h"
#include "DensityRenderer.h"
#include "DensityFormatter.h"
#include "DensityClickHandler.h"

DensityFactory::DensityFactory(DataThread *data_thread) : VisualizationFactory(data_thread) {

}


Renderer *DensityFactory::create_renderer() const {
    return new DensityRenderer();
}

AxisFormatter *DensityFactory::create_formatter() const {
    return new DensityFormatter();
}

ClickHandler *DensityFactory::create_click_handler() const {
    return new DensityClickHandler(get_data_thread());
}
