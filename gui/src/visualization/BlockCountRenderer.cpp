#include "BlockCountRenderer.h"
#include "CanvasLineObject.h"
#include "storage/AllocEvent.h"
#include "storage/FreeEvent.h"
#include "CanvasGraphObject.h"

BlockCountRenderer::BlockCountRenderer(Canvas *canvas) : Renderer(canvas), block_count(0.0) {
}

BlockCountRenderer::~BlockCountRenderer() {

}

void BlockCountRenderer::visit(AllocEvent *event) {
    block_count += 1.0;
    /*canvas->add_object(new CanvasLineObject(DataPoint(event->get_timestamp(), block_count), DataPoint(event->get_timestamp(), 0), qRgb(0, 0, 0)));*/
    DataRange line = DataRange(DataPoint(event->get_timestamp(), block_count), point);
    canvas->add_object(new CanvasLineObject(line.get_begin(), line.get_end(), qRgb(0, 0, 0)));
    canvas->add_updated_range(line);
    
    point = DataPoint(event->get_timestamp(), block_count);
}

void BlockCountRenderer::visit(FreeEvent *event) {
    block_count -= 1.0;
    /*canvas->add_object(new CanvasLineObject(DataPoint(event->get_timestamp(), block_count), DataPoint(event->get_timestamp(), 0), qRgb(0, 0, 0)));*/
    DataRange line = DataRange(DataPoint(event->get_timestamp(), block_count), point);
    canvas->add_object(new CanvasLineObject(line.get_begin(), line.get_end(), qRgb(0, 0, 0)));
    canvas->add_updated_range(line);
    point = DataPoint(event->get_timestamp(), block_count);
}
