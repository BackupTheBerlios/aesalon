#include <QTimer>
#include "DataSourceManager.h"
#include "DataSourceManager.moc"
#include "data/DataSourceParser.h"

DataSourceManager::DataSourceManager(QWidget *parent) : QWidget(parent) {
    main_layout = new QVBoxLayout();
    
    load_from_config();
    
    source_list = new QListWidget();
    main_layout->addWidget(source_list);
    
    QTimer::singleShot(0, this, SLOT(update_list()));
    
    this->setLayout(main_layout);
}

DataSourceManager::~DataSourceManager() {
    save_to_config();
}


void DataSourceManager::add_new_data_source(DataSource *data_source) {
    data_source_list.append(data_source);
    source_list->addItem(data_source->get_name());
}

void DataSourceManager::data_source_changed(DataSource *data_source) {
    /*data_source_list[data_source_list.indexOf(data_source)]->get_name();*/
}

void DataSourceManager::load_from_config() {
    data_source_list.clear();
    QSettings settings;
    int size = settings.beginReadArray("data-sources");
    
    for(int i = 0; i < size; i ++) {
        settings.setArrayIndex(i);
        DataSource *ds = DataSourceParser::parse_data_source(&settings);
        if(ds) data_source_list.append(ds);
    }
    
    settings.endArray();
}

void DataSourceManager::save_to_config() {
    QSettings settings;
    settings.beginWriteArray("data-sources");
    
    int i = 0;
    foreach(DataSource *ds, data_source_list) {
        settings.setArrayIndex(i++);
        if(ds) ds->save_to(&settings);
    }
    
    settings.endArray();
}

void DataSourceManager::update_list() {
    source_list->clear();
    foreach(DataSource *ds, data_source_list) {
        source_list->addItem(ds->get_name());
    }
}
