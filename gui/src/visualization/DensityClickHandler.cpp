#include <QWhatsThis>
#include "DensityClickHandler.h"

DensityClickHandler::DensityClickHandler(DataThread *data_thread) : ClickHandler(), data_thread(data_thread) {
}

DensityClickHandler::~DensityClickHandler() {
}

void DensityClickHandler::handle_click(Canvas *canvas, DataPoint at) {
    QString display;
    CanvasObject *object = canvas->object_at(at);
    if(object == NULL) {
        qDebug("Couldn't find CanvasObject for specified point");
        return;
    }
    
    Block *block = data_thread->get_snapshot_list()->get_block(at.get_time_element(), quint64(object->get_bounding_rect().get_begin().get_data_element()));
    /*Block *block = data_thread->get_snapshot_list()->get_block(at.get_time_element(), quint64(at.get_data_element()));*/
    if(block == NULL) {
        qDebug("Block for specified point not found");
        return;
    }
    display.sprintf("Allocated at: %s\nAllocation scope: %llx",
        qPrintable(block->get_allocation_time().to_string()), block->get_allocation_scope());
    QWhatsThis::showText(QPoint(0, 0), display);
}
