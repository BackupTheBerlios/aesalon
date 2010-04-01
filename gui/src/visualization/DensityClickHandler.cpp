#include <QWhatsThis>
#include "DensityClickHandler.h"

DensityClickHandler::DensityClickHandler(DataThread *data_thread) : ClickHandler(), data_thread(data_thread) {
}

DensityClickHandler::~DensityClickHandler() {
}

void DensityClickHandler::handle_click(DataPoint at) {
    QString display;
    Block *block = data_thread->get_snapshot_list()
        ->get_block_for(at.get_time_element(), quint64(at.get_data_element()));
    if(block == NULL) {
        return;
    }
    display.sprintf("Allocated at: %s\nAllocation scope: %llx",
        qPrintable(block->get_allocation_time().to_string()), block->get_allocation_scope());
    QWhatsThis::showText(QPoint(0, 0), display);
}
