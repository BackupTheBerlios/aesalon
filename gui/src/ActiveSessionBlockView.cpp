#include "ActiveSessionBlockView.h"
#include "ActiveSessionBlockView.moc"
#include "ActiveSessionMemoryStorage.h"

ActiveSessionBlockView::ActiveSessionBlockView(QWidget *parent, ActiveSessionMemoryStorage *storage) : QWidget(parent), storage(storage) {
    finish_time.setTime_t(0);
    start_time.setTime_t(0);
    
    main_layout = new QVBoxLayout();
    
    slider_layout = new QHBoxLayout();
    current_time = new QLabel("0:00");
    slider_layout->addWidget(current_time);
    time_slider = new QSlider(Qt::Horizontal);
    time_slider->setRange(0, 1);
    time_slider->setDisabled(true);
    connect(time_slider, SIGNAL(sliderMoved(int)), this, SLOT(slider_value_change(int)));
    connect(time_slider, SIGNAL(valueChanged(int)), this, SLOT(slider_value_change(int)));
    connect(time_slider, SIGNAL(sliderReleased()), this, SLOT(slider_released()));

    update_timer = new QTimer();
    connect(update_timer, SIGNAL(timeout()), this, SLOT(recalc_slider()));

    slider_layout->addWidget(time_slider);
    
    realtime_checkbox = new QCheckBox("&Realtime");
    realtime_checkbox->setCheckState(Qt::Checked);
    connect(realtime_checkbox, SIGNAL(toggled(bool)), this, SLOT(realtime_checkbox_toggled(bool)));
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
    time_slider->setEnabled(!new_state);
    emit request_realtime(new_state);
    if(!new_state) recalc_slider();
}

void ActiveSessionBlockView::recalc_slider() {
    if(!update_timer->isActive()) {
        time_slider->setRange(0, start_time.secsTo(finish_time)+1);
    }
    else {
        time_slider->setRange(0, start_time.secsTo(QDateTime::currentDateTime()));
        if(!time_slider->isEnabled()) {
            time_slider->setValue(start_time.secsTo(QDateTime::currentDateTime()));
            slider_value_change(start_time.secsTo(QDateTime::currentDateTime()));
        }
    }
}

void ActiveSessionBlockView::slider_value_change(int new_value) {
    QString new_text;
    new_text.setNum(new_value / 60);
    new_text += ":";
    if((new_value % 60) < 10) new_text += "0";
    new_text += QString().setNum(new_value % 60);
    current_time->setText(new_text);
}

void ActiveSessionBlockView::slider_released() {
    QDateTime new_time = start_time.addSecs(time_slider->value());
    emit request_time_data(new_time);
}

void ActiveSessionBlockView::update_content(ActiveSessionMemorySnapshot *memory) {
    QSet<StorageOffset> blocks = memory->get_content();
    
    block_table->setSortingEnabled(false);
    
    foreach(StorageOffset offset, blocks) {
        ActiveSessionMemoryBlock *block = storage->get_block_at(qAbs<StorageOffset>(offset));
        QString address_string = "0x" + QString().setNum(block->get_address(), 16);
        if(offset >= 0) {
            QTableWidgetItem *item = new QTableWidgetItem(address_string);
            block_table->setRowCount(block_table->rowCount()+1);
            block_table->setItem(block_table->rowCount()-1, 0, item);
            item = new QTableWidgetItem(QString().setNum(block->get_size()));
            block_table->setItem(block_table->rowCount()-1, 1, item);
            block_table->resizeRowToContents(block_table->rowCount()-1);
        }
        else {
            QList<QTableWidgetItem *> items = block_table->findItems(address_string, Qt::MatchExactly);
            foreach(QTableWidgetItem *item, items) block_table->removeRow(item->row());
        }
    }
    
    block_table->setSortingEnabled(true);
#if 0
    ActiveSessionMemorySnapshot *change_difference = NULL;
    ActiveSessionMemorySnapshot *remove_difference = NULL;
    change_difference = memory->find_changed(displayed_memory);
    remove_difference = memory->find_removed(displayed_memory);
    
    for(int x = 0; x < change_difference->get_blocks(); x ++) {
        QList<QTableWidgetItem *> items = block_table->findItems("0x" + QString().setNum(change_difference->get_block_by_index(x)->get_address(), 16), Qt::MatchExactly);
        /* If it already exists, then it's a resize */
        if(items.size()) {
            /*block_table->item(items[0]->row(), 1)->setData(Qt::DisplayRole, QString().setNum(change_difference->get_block_by_index(x)->get_size()));*/
            block_table->item(items[0]->row(), 1)->setText(QString().setNum(change_difference->get_block_by_index(x)->get_size()));
            /* TODO: find a better way of forcing a repaint . . . */
            block_table->setRowHeight(items[0]->row(), 1);
            block_table->resizeRowToContents(items[0]->row());
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
#endif
}

void ActiveSessionBlockView::started(QDateTime time) {
    start_time = time;
    update_timer->start(1000);
}

void ActiveSessionBlockView::finished(QDateTime time) {
    finish_time = time;
    update_timer->stop();
    realtime_checkbox->setEnabled(false);
    realtime_checkbox->setChecked(false);
    recalc_slider();
}
