#include "BlockCountRenderer.h"
#include "CanvasLineObject.h"
#include "storage/AllocEvent.h"
#include "storage/FreeEvent.h"
#include "CanvasGraphObject.h"

BlockCountRenderer::BlockCountRenderer() {

}

BlockCountRenderer::~BlockCountRenderer() {

}

void BlockCountRenderer::begin_rendering(const DataRange &render_range, Snapshot *snapshot) {
    block_count = 0;
    count_blocks(snapshot->get_head_node());
    canvas = new Canvas(render_range);
    point = DataPoint(render_range.get_begin().get_time_element(), block_count);
    canvas->add_object(new CanvasLineObject(point, DataPoint(render_range.get_begin().get_time_element(), 0), qRgb(0, 0, 0)));
}

Canvas *BlockCountRenderer::end_rendering() {
    canvas->add_object(new CanvasLineObject(
        DataPoint(canvas->get_range().get_end().get_time_element(), block_count),
        DataPoint(canvas->get_range().get_end().get_time_element(), 0), qRgb(0, 0, 0)));
    return canvas;
}

void BlockCountRenderer::visit(AllocEvent *event) {
    block_count += 1.0;
    canvas->add_object(new CanvasLineObject(DataPoint(event->get_timestamp(), block_count), DataPoint(event->get_timestamp(), 0), qRgb(0, 0, 0)));
    canvas->add_object(new CanvasLineObject(DataPoint(event->get_timestamp(), block_count), point, qRgb(0, 0, 0)));
    point = DataPoint(event->get_timestamp(), block_count);
}

void BlockCountRenderer::visit(FreeEvent *event) {
    block_count -= 1.0;
    canvas->add_object(new CanvasLineObject(DataPoint(event->get_timestamp(), block_count), DataPoint(event->get_timestamp(), 0), qRgb(0, 0, 0)));
    canvas->add_object(new CanvasLineObject(DataPoint(event->get_timestamp(), block_count), point, qRgb(0, 0, 0)));
    point = DataPoint(event->get_timestamp(), block_count);
}

void BlockCountRenderer::count_blocks(BiTreeNode *node) {
    if(node->get_left()) count_blocks(node->get_left());
    if(node->get_right()) count_blocks(node->get_right());
    block_count += node->get_block_list_size();
}
