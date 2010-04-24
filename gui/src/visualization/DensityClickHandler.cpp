#include <QWhatsThis>
#include <QTextStream>
#include <QFrame>
#include "DensityClickHandler.h"

DensityClickHandler::DensityClickHandler(DataThread *data_thread, QWidget *info_widget) : ClickHandler(info_widget), data_thread(data_thread) {
    QHBoxLayout *main_layout = new QHBoxLayout();
    
    QFormLayout *generic = new QFormLayout();
    
    address = new QLabel(QObject::tr("N/A"));
    generic->addRow(QObject::tr("Address:"), address);
    size = new QLabel(QObject::tr("N/A"));
    generic->addRow(QObject::tr("Size:"), size);
    
    main_layout->addLayout(generic);
    
    QFormLayout *allocation = new QFormLayout();
    
    alloc_time = new QLabel(QObject::tr("N/A"));
    allocation->addRow(QObject::tr("Allocation time:"), alloc_time);
    alloc_scope = new QLabel(QObject::tr("N/A"));
    allocation->addRow(QObject::tr("Allocation scope:"), alloc_scope);
    
    main_layout->addLayout(allocation);
    
    QFormLayout *release = new QFormLayout();
    
    release_time = new QLabel(QObject::tr("N/A"));
    release->addRow(QObject::tr("Release time:"), release_time);
    release_scope = new QLabel(QObject::tr("N/A"));
    release->addRow(QObject::tr("Release scope:"), release_scope);
    
    main_layout->addLayout(release);
    
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
    alloc_scope->setText(block->get_allocation_scope().get_name());
    
    if(block->get_release_time() == Timestamp::NOW) {
        release_time->setText(QObject::tr("Still active"));
        release_scope->setText(QObject::tr("Still active"));
    }
    else {
        release_time->setText(block->get_release_time().to_string());
        release_scope->setText(block->get_release_scope().get_name());
    }
    
    /*
    stream << "Address:\t\t0x" << hex << block->get_address() << "\n";
    stream << "Size:\t\t" << dec << block->get_size() << "\n";
    stream << "Allocation time:\t" << block->get_allocation_time().to_string() << "\n";
    stream << "Allocation scope:\t0x" << hex << block->get_allocation_scope().get_name() << "\n";
    
    if(block->get_release_time().to_ns() != Timestamp::NOW) {
        stream << "Release time:\t" << block->get_release_time().to_string() << "\n";
    }
    else {
        stream << "Release time:\tN/A\n";
    }
    if(block->get_release_scope()) {
        stream << "Release scope:\t" << hex << block->get_release_scope().get_name() << "\n";
    }
    else {
        stream << "Release scope:\tN/A\n";
    }
    if(block->get_release_time().to_ns() != Timestamp::NOW) {
        stream << "Total time allocated:\t" << Timestamp(block->get_allocation_time().ns_until(block->get_release_time())).to_string() << "\n";
    }
    else {
        stream << "Total time allocated:\t" << Timestamp(block->get_allocation_time().ns_until(object->get_bounding_rect().get_end().get_time_element())).to_string() << "\n";
    }*/
    
    /*display.sprintf("Allocated at: %s\nAllocation scope: %llx",
        qPrintable(block->get_allocation_time().to_string()), block->get_allocation_scope());*/
    /*QWhatsThis::showText(QPoint(0, 0), display);*/
}
