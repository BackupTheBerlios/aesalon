#include "ActiveSessionBlockView.h"
#include "ActiveSessionBlockView.moc"

ActiveSessionBlockView::ActiveSessionBlockView(QWidget *parent) : QWidget(parent) {
    displayed_memory = new ActiveSessionMemorySnapshot();
    
    main_layout = new QVBoxLayout();
    
    slider_layout = new QHBoxLayout();
    current_time = new QLabel("00:00");
    slider_layout->addWidget(current_time);
    time_slider = new QSlider(Qt::Horizontal);
    time_slider->setRange(0, 1);

    slider_layout->addWidget(time_slider);
    
    realtime_checkbox = new QCheckBox("&Realtime");
    realtime_checkbox->setCheckState(Qt::Checked);
    connect(realtime_checkbox, SIGNAL(toggled(bool)), this, SIGNAL(request_realtime(bool)));
    slider_layout->addWidget(realtime_checkbox);
    
    main_layout->addLayout(slider_layout);
    
    block_table = new QTableWidget();
    block_table->setColumnCount(2);
    block_table->setRowCount(0);
    block_table->setShowGrid(true);
    block_table->setGridStyle(Qt::DotLine);
    block_table->setSortingEnabled(true);
    QStringList horizontal_labels;
    horizontal_labels << "Address" << "Size";
    block_table->setHorizontalHeaderLabels(horizontal_labels);
    block_table->setColumnWidth(0, 200);
    block_table->setColumnWidth(1, 80);
    block_table->clearContents();
    block_table->setSelectionBehavior(QTableWidget::SelectRows);
    block_table->setSelectionMode(QTableWidget::SingleSelection);
    main_layout->addWidget(block_table);
    
    setLayout(main_layout);
}

void ActiveSessionBlockView::realtime_checkbox_toggled(bool new_state) {
    time_slider->setDisabled(new_state);
    emit request_realtime(new_state);
}

void ActiveSessionBlockView::update_content(ActiveSessionMemorySnapshot *memory) {
    ActiveSessionMemorySnapshot *change_difference = NULL;
    ActiveSessionMemorySnapshot *remove_difference = NULL;
    change_difference = memory->find_changed(displayed_memory);
    remove_difference = memory->find_removed(displayed_memory);
    
    for(int x = 0; x < change_difference->get_blocks(); x ++) {
        QList<QTableWidgetItem *> items = block_table->findItems("0x" + QString().setNum(change_difference->get_block_by_index(x)->get_address(), 16), Qt::MatchExactly);
        /* If it already exists, then it's a resize */
        if(items.size()) {
            block_table->item(items[0]->row(), 1)->setText(QString().setNum(change_difference->get_block_by_index(x)->get_size()));
        }
        /* Otherwise, add it . . . */
        else {
            QTableWidgetItem *item = new QTableWidgetItem("0x" + QString().setNum(change_difference->get_block_by_index(x)->get_address(), 16));
            block_table->setRowCount(block_table->rowCount()+1);
            block_table->setItem(block_table->rowCount()-1, 0, item);
            item = new QTableWidgetItem(QString().setNum(change_difference->get_block_by_index(x)->get_size()));
            block_table->setItem(block_table->rowCount()-1, 1, item);
            block_table->resizeRowToContents(block_table->rowCount()-1);
        }
    }
    for(int x = 0; remove_difference && x < remove_difference->get_blocks(); x ++) {
        QList<QTableWidgetItem *> items = block_table->findItems("0x" + QString().setNum(remove_difference->get_block_by_index(x)->get_address(), 16), Qt::MatchExactly);
        if(items.size()) block_table->removeRow(items[0]->row());
    }
    delete remove_difference;
    delete change_difference;
    delete displayed_memory;
    displayed_memory = memory->clone();
}
