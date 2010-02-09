#ifndef AESALON_GUI_MAIN_DATA_SOURCE_MANAGER_H
#define AESALON_GUI_MAIN_DATA_SOURCE_MANAGER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>

#include "data/DataSource.h"

class DataSourceManager : public QWidget { Q_OBJECT
private:
    QVBoxLayout *main_layout;
    QListWidget *source_list;
    QList<DataSource *> data_source_list;
public:
    DataSourceManager(QWidget *parent);
public slots:
    void add_new_data_source(DataSource *data_source);
    void data_source_changed(DataSource *data_source);
};

#endif
