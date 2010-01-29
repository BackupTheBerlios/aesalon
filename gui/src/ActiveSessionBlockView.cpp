#include "ActiveSessionBlockView.h"
#include "ActiveSessionBlockView.moc"

ActiveSessionBlockView::ActiveSessionBlockView(QWidget *parent) : QTableWidget(parent) {
    this->setColumnCount(2);
    this->setRowCount(0);
    this->setShowGrid(false);
    this->setSortingEnabled(false);
    QStringList horizontal_labels;
    horizontal_labels << "Address" << "Size";
    this->setHorizontalHeaderLabels(horizontal_labels);
    this->clearContents();
    this->setSelectionBehavior(SelectRows);
    this->setSelectionMode(SingleSelection);
}

void ActiveSessionBlockView::process_diff(ActiveSessionMemoryBlockDiff *diff) {
    if(diff->get_type() == ActiveSessionMemoryBlockDiff::ALLOC_DIFF) {
        this->setRowCount(this->rowCount()+1);
        QTableWidgetItem *item = new QTableWidgetItem(QString().setNum(diff->get_address(), 16));
        this->setItem(this->rowCount()-1, 0, item);
        item = new QTableWidgetItem(QString().setNum(diff->get_size()));
        this->setItem(this->rowCount()-1, 1, item);
        this->resizeRowToContents(this->rowCount()-1);
    }
    else if(diff->get_type() == ActiveSessionMemoryBlockDiff::FREE_DIFF) {
        QList<QTableWidgetItem *> items = this->findItems(QString().setNum(diff->get_address(), 16), Qt::MatchExactly);
        if(items.size()) this->removeRow(items[0]->row());
    }
}