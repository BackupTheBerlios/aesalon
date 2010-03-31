#ifndef AESALON_GUI_MAIN_DATA_SOURCE_EDITOR_H
#define AESALON_GUI_MAIN_DATA_SOURCE_EDITOR_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>

#include "data/DataSource.h"

class DataSourceEditor : public QDialog { Q_OBJECT
private:
    DataSource *data_source;
    QFormLayout *main_layout;
    QLineEdit *source_name;
    QTabWidget *tab_bar;
    QWidget *network_tab;
    QFormLayout *network_layout;
    QLineEdit *network_host;
    QSpinBox *network_port;
    QDialogButtonBox *buttons;
public:
    DataSourceEditor(QWidget* parent);
    virtual ~DataSourceEditor();
public slots:
    DataSource *create_new();
    void edit(DataSource *data_source);
    void accept();
};

#endif
