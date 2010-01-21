#include "ActiveSessionBlockView.h"
#include "ActiveSessionBlockView.moc"
#include "platform/BlockEvent.h"

namespace Aesalon {
namespace GUI {

ActiveSessionBlockView::ActiveSessionBlockView(Platform::Memory *memory, QWidget *parent) : QTableWidget(parent), memory(memory) {
    this->setColumnCount(2);
    QStringList headers;
    headers << "Address" << "Size";
    this->setHorizontalHeaderLabels(headers);
    this->setSortingEnabled(true);
    this->setColumnWidth(0, 400);
    this->setColumnWidth(1, 100);
    this->setShowGrid(false);
    this->setSelectionMode(SingleSelection);
    this->setSelectionBehavior(SelectRows);
    this->sortByColumn(0, Qt::AscendingOrder);
}

ActiveSessionBlockView::~ActiveSessionBlockView() {

}

void ActiveSessionBlockView::memory_changed(Platform::Event *event) {
    /* This is the block view, don't care about non-block events . . . */
    if(event->get_type() != Platform::Event::BLOCK_EVENT) return;
    Platform::BlockEvent *be = dynamic_cast<Platform::BlockEvent *>(event);
    /*this->addItem(QString().setNum(be->get_address()));*/
    if(be->get_block_type() == Platform::BlockEvent::ALLOC_EVENT) {
        this->setRowCount(this->rowCount()+1);
        QTableWidgetItem *address, *size;
        address = new QTableWidgetItem("0x" + QString().setNum((quint64)be->get_address(), 16));
        address->setData(Qt::UserRole, (quint64)be->get_address());
        size = new QTableWidgetItem(QString().setNum((quint64)be->get_size()));
        size->setData(Qt::UserRole, (quint64)be->get_size());
        this->setItem(this->rowCount()-1, 0, address);
        this->setItem(this->rowCount()-1, 1, size);
    }
    else if(be->get_block_type() == Platform::BlockEvent::FREE_EVENT) {
        QList<QTableWidgetItem *> matches = this->findItems("0x" + QString().setNum((quint64)be->get_address(), 16), Qt::MatchExactly);
        if(matches.size()) this->removeRow(matches[0]->row());
    }
    else if(be->get_block_type() == Platform::BlockEvent::REALLOC_EVENT) {
        QList<QTableWidgetItem *> matches = this->findItems("0x" + QString().setNum((quint64)be->get_address(), 16), Qt::MatchExactly);
        if(matches.size()) {
            QTableWidgetItem *address = this->item(matches[0]->row(), 0);
            address->setData(Qt::UserRole, (quint64)be->get_new_address());
            address->setText("0x" + QString().setNum((quint64)be->get_new_address(), 16));
            
            QTableWidgetItem *size = this->item(matches[0]->row(), 1);
            size->setData(Qt::UserRole, (quint64)be->get_size());
            size->setText(QString().setNum((quint64)be->get_size()));
        }
    }
}

} // namespace GUI
} // namespace Aesalon
