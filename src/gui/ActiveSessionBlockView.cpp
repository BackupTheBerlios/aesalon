#include "ActiveSessionBlockView.h"
#include "ActiveSessionBlockView.moc"
#include "platform/BlockEvent.h"

namespace Aesalon {
namespace GUI {

ActiveSessionBlockView::ActiveSessionBlockView(Platform::Memory *memory, QWidget *parent) : QTableWidget(parent), memory(memory) {
    this->setColumnCount(2);
    
}

ActiveSessionBlockView::~ActiveSessionBlockView() {

}

void ActiveSessionBlockView::memory_changed(Platform::Event *event) {
    /* This is the block view, don't care about non-block events . . . */
    if(event->get_type() != Platform::Event::BLOCK_EVENT) return;
    Platform::BlockEvent *be = dynamic_cast<Platform::BlockEvent *>(event);
    /*this->addItem(QString().setNum(be->get_address()));*/
    this->setRowCount(this->rowCount()+1);
    QTableWidgetItem *address, *size;
    address = new QTableWidgetItem(QString().setNum((quint64)be->get_address(), 16));
    size = new QTableWidgetItem(QString().setNum((quint64)be->get_size()));
    this->setItem(this->rowCount()-1, 0, address);
    this->setItem(this->rowCount()-1, 1, size);
    
}

} // namespace GUI
} // namespace Aesalon
