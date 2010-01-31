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
    displayed_memory = new ActiveSessionMemorySnapshot();
}

void ActiveSessionBlockView::update_content(ActiveSessionMemorySnapshot *memory) {
    ActiveSessionMemorySnapshot *change_difference = NULL;
    ActiveSessionMemorySnapshot *remove_difference = NULL;
    change_difference = memory->find_changed(displayed_memory);
    remove_difference = memory->find_removed(displayed_memory);
    qDebug("changed: %i, removed: %i", change_difference->get_blocks(), remove_difference->get_blocks());
    
    for(int x = 0; x < change_difference->get_blocks(); x ++) {
        QList<QTableWidgetItem *> items = findItems("0x" + QString().setNum(change_difference->get_block_by_index(x)->get_address(), 16), Qt::MatchExactly);
        /* If it already exists, then it's a resize */
        if(items.size()) {
            item(items[0]->row(), 1)->setText(QString().setNum(change_difference->get_block_by_index(x)->get_size()));
        }
        /* Otherwise, add it . . . */
        else {
            QTableWidgetItem *item = new QTableWidgetItem("0x" + QString().setNum(change_difference->get_block_by_index(x)->get_address(), 16));
            setRowCount(rowCount()+1);
            setItem(rowCount()-1, 0, item);
            item = new QTableWidgetItem(QString().setNum(change_difference->get_block_by_index(x)->get_size()));
            setItem(rowCount()-1, 1, item);
            resizeRowToContents(rowCount()-1);
        }
    }
    for(int x = 0; remove_difference && x < remove_difference->get_blocks(); x ++) {
        QList<QTableWidgetItem *> items = findItems("0x" + QString().setNum(remove_difference->get_block_by_index(x)->get_address(), 16), Qt::MatchExactly);
        if(items.size()) removeRow(items[0]->row());
    }
    delete remove_difference;
    delete change_difference;
    delete displayed_memory;
    displayed_memory = memory->clone();
}
