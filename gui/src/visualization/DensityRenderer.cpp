#include "DensityRenderer.h"
#include "CanvasDensityObject.h"
#include "storage/AllocEvent.h"
#include "storage/FreeEvent.h"

DensityRenderer::DensityRenderer(Canvas *canvas) : Renderer(canvas) {

}

DensityRenderer::~DensityRenderer() {

}

void DensityRenderer::visit(AllocEvent *event) {
    CanvasObject *object =
        new CanvasDensityObject(DataRange(event->get_timestamp(), event->get_address(),
            Timestamp(Timestamp::NOW), event->get_address() + event->get_size()), qRgb(0, 0, 0));
    blocks[event->get_address()] = object;
    canvas->add_object(object);
    canvas->add_updated_range(object->get_bounding_rect());
}

void DensityRenderer::visit(FreeEvent *event) {
    CanvasObject *object = blocks.take(event->get_address());
    object->get_bounding_rect().get_end().set_time_element(event->get_timestamp());
    canvas->add_updated_range(object->get_bounding_rect());
}
