#include <QWhatsThis>
#include <QTextStream>
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
    QTextStream stream(&display);
    
    /*display.sprintf("Address:\t\t0x%llx\nSize:\t\t%lli\nAllocation time:\t%s\nAllocation scope:\t0x%llx\n",
        block->get_address(), block->get_size(), qPrintable(block->get_allocation_time().to_string()), block->get_allocation_scope());*/
    
    stream << "Address:\t\t" << hex << block->get_address() << "\n";
    stream << "Size:\t\t" << dec << block->get_size() << "\n";
    stream << "Allocation time:\t" << block->get_allocation_time().to_string() << "\n";
    stream << "Allocation scope:\t" << hex << block->get_allocation_scope() << "\n";
    
    if(block->get_release_time().to_ns() != 0) {
        stream << "Release time:\t" << block->get_release_time().to_string() << "\n";
    }
    else {
        stream << "Release time:\tN/A\n";
    }
    if(block->get_release_scope()) {
        stream << "Release scope:\t" << hex << block->get_release_scope() << "\n";
    }
    else {
        stream << "Release scope:\tN/A\n";
    }
    stream << "Total time allocated:\t" << Timestamp(block->get_allocation_time().ns_until(block->get_release_time())).to_string() << "\n";
    
    /*display.sprintf("Allocated at: %s\nAllocation scope: %llx",
        qPrintable(block->get_allocation_time().to_string()), block->get_allocation_scope());*/
    QWhatsThis::showText(QPoint(0, 0), display);
}
