#include "Renderer.h"
#include "CanvasTimelineObject.h"

Renderer::Renderer(Canvas *canvas) : canvas(canvas) {
    canvas->add_object(new CanvasTimelineObject(Timestamp(Timestamp::NOW)));
}

Renderer::~Renderer() {

}

void Renderer::update_timeline(const Timestamp &timestamp) {
    canvas->get_head()->get_bounding_rect().get_end().set_time_element(timestamp);
}
