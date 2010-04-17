#include "DensityRenderer.h"
#include "CanvasDensityObject.h"
#include "storage/AllocEvent.h"
#include "storage/FreeEvent.h"

DensityRenderer::DensityRenderer() {

}

DensityRenderer::~DensityRenderer() {

}

void DensityRenderer::begin_rendering(const DataRange &render_range, Snapshot *snapshot) {
    canvas = new Canvas(render_range);
    
    assemble_blocks(snapshot->get_head_node());
}

Canvas *DensityRenderer::end_rendering() {
    foreach(DataRange range, blocks) {
        range.get_end().set_time_element(canvas->get_range().get_end().get_time_element());
        canvas->add_object(new CanvasDensityObject(range, qRgb(0, 0, 0)));
    }
    return canvas;
}

void DensityRenderer::visit(AllocEvent* event) {
    blocks[event->get_address()] = DataRange(event->get_timestamp(), event->get_address(),
        Timestamp(0), event->get_address() + event->get_size());
}

void DensityRenderer::visit(FreeEvent* event) {
    DataRange range = blocks[event->get_address()];
    blocks.remove(event->get_address());
    range.get_end().set_time_element(event->get_timestamp());
    canvas->add_object(new CanvasDensityObject(range, qRgb(0, 0, 0)));
}

void DensityRenderer::assemble_blocks(BiTreeNode* node) {
    if(node->get_left()) assemble_blocks(node->get_left());
    if(node->get_right()) assemble_blocks(node->get_right());
    foreach(Block *block, node->get_block_list()) {
        blocks[block->get_address()] = DataRange(block->get_allocation_time(), block->get_address(),
            Timestamp(0), block->get_address() + block->get_size());
    }
}
