#include "DataSourceManager.h"
#include "DataSourceManager.moc"

DataSourceManager::DataSourceManager(QWidget *parent) : QWidget(parent) {
    main_layout = new QVBoxLayout();
    
    source_list = new QListWidget();
    
    main_layout->addWidget(source_list);
    
    this->setLayout(main_layout);
}

void DataSourceManager::add_new_data_source(DataSource *data_source) {
    data_source_list.append(data_source);
    source_list->addItem(data_source->get_name());
}

void DataSourceManager::data_source_changed(DataSource *data_source) {
    /*data_source_list[data_source_list.indexOf(data_source)]->get_name();*/
}
