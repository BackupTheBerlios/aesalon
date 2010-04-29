#include <QWhatsThis>
#include <QTextStream>
#include <QFrame>
#include "DensityClickHandler.h"

DensityClickHandler::DensityClickHandler(DataThread *data_thread, QWidget *info_widget) : ClickHandler(info_widget), data_thread(data_thread) {
    QFormLayout *main_layout = new QFormLayout();
    
    address = new QLabel(QObject::tr("N/A"));
    main_layout->addRow(QObject::tr("Address:"), address);
    size = new QLabel(QObject::tr("N/A"));
    main_layout->addRow(QObject::tr("Size:"), size);
    
    
    
    alloc_time = new QLabel(QObject::tr("N/A"));
    main_layout->addRow(QObject::tr("Allocation time:"), alloc_time);
    alloc_scope = new QListWidget();
    main_layout->addRow(QObject::tr("Allocation scope:"), alloc_scope);
    
    release_time = new QLabel(QObject::tr("N/A"));
    main_layout->addRow(QObject::tr("Release time:"), release_time);
    release_scope = new QListWidget();
    main_layout->addRow(QObject::tr("Release scope:"), release_scope);
    
    info_widget->setLayout(main_layout);
}

DensityClickHandler::~DensityClickHandler() {
    
}

void DensityClickHandler::handle_click(Canvas *canvas, DataPoint at) {
    QString display;
    CanvasObject *object = canvas->object_at(at);
    if(object == NULL) {
        return;
    }
    
    Block *block = data_thread->get_snapshot_list()->get_block(at.get_time_element(), quint64(object->get_bounding_rect().get_begin().get_data_element()));
    /*Block *block = data_thread->get_snapshot_list()->get_block(at.get_time_element(), quint64(at.get_data_element()));*/
    if(block == NULL) {
        return;
    }
    QTextStream stream(&display);
    
    /*display.sprintf("Address:\t\t0x%llx\nSize:\t\t%lli\nAllocation time:\t%s\nAllocation scope:\t0x%llx\n",
        block->get_address(), block->get_size(), qPrintable(block->get_allocation_time().to_string()), block->get_allocation_scope());*/
    
    address->setText(QString("0x") + QString().setNum(block->get_address(), 16));
    size->setText(QString().setNum(block->get_size(), 10));
    
    alloc_time->setText(block->get_allocation_time().to_string());
    alloc_scope->clear();
    Backtrace allocation = block->get_allocation_scope();
    for(int i = 0; i < allocation.get_scope_list_size(); i ++) {
        alloc_scope->insertItem(0, allocation.get_scope_list()[i].get_name());
    }
    
    if(block->get_release_time() == Timestamp::NOW) {
        release_time->setText(QObject::tr("Still active"));
        release_scope->clear();
        release_scope->addItem("Still active");
        /*release_scope->setText(QObject::tr("Still active"));*/
    }
    else {
        release_time->setText(block->get_release_time().to_string());
        release_scope->clear();
        Backtrace release = block->get_release_scope();
        for(int i = 0; i < release.get_scope_list_size(); i ++) {
            release_scope->addItem(release.get_scope_list()[i].get_name());
        }
    }
}
