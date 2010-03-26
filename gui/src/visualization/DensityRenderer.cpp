#include "DensityRenderer.h"
#include "DensityRenderer.moc"
#include "storage/AllocEvent.h"
#include "storage/FreeEvent.h"

DensityRenderer::DensityRenderer(QImage* image): VisualizationRenderer(image, false) {

}

DensityRenderer::~DensityRenderer() {

}

void DensityRenderer::begin_update(Snapshot *starting_snaphot) {
    gather_blocks(starting_snaphot->get_head_node());
    VisualizationRenderer::begin_update(starting_snaphot);
}

void DensityRenderer::end_update() {
    foreach(VisualizationDataRange range, blocks) {
        range.set_upper_time(range.get_upper_time());
        paint_box(range, qRgb(0, 0, 0), qRgb(96, 96, 96));
    }
    VisualizationRenderer::end_update();
}

void DensityRenderer::gather_blocks(BiTreeNode* node) {
    if(node->get_left()) gather_blocks(node->get_left());
    if(node->get_right()) gather_blocks(node->get_right());
    if(node->get_block_list_size()) {
        foreach(Block *block, node->get_block_list()) {
            blocks[block->get_address()] = VisualizationDataRange(block->get_allocation_time(), Timestamp(0),
                block->get_address(), block->get_address() + block->get_size());
        }
    }
}

void DensityRenderer::visit(AllocEvent *event) {
    qDebug("DensityRenderer::visit(AllocEvent) . . .");
    blocks[event->get_address()] = VisualizationDataRange(event->get_timestamp(), Timestamp(0),
        event->get_address(), event->get_address() + event->get_size());
}

void DensityRenderer::visit(FreeEvent *event) {
    qDebug("DensityRenderer::visit(FreeEvent) . . .");
    VisualizationDataRange range = blocks[event->get_address()];
    range.set_upper_time(event->get_timestamp());
    paint_box(range, qRgb(0, 0, 0), qRgb(96, 96, 96));
    blocks.remove(event->get_address());
}
