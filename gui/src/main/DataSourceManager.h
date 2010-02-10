#ifndef AESALON_GUI_MAIN_DATA_SOURCE_MANAGER_H
#define AESALON_GUI_MAIN_DATA_SOURCE_MANAGER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>

#include "data/DataSource.h"
#include "session/Session.h"

class DataSourceManager : public QWidget { Q_OBJECT
private:
    QVBoxLayout *main_layout;
    QListWidget *source_list;
    QList<DataSource *> data_source_list;
    QHBoxLayout *lower_button_layout;
    QPushButton *new_source;
    QPushButton *create_session;
public:
    DataSourceManager(QWidget *parent);
    virtual ~DataSourceManager();
private slots:
    void load_from_config();
    void save_to_config();
    void update_list();
    void launch_session();
public slots:
    void add_new_data_source(DataSource *data_source);
    void data_source_changed(DataSource *data_source);
signals:
    void session_launched(Session *session);
    void tab_request(QWidget *widget, QString name);
};

#endif
