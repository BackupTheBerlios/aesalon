#ifndef AESALON_GUI_VISUALIZATION_FILTER_EDITOR_H
#define AESALON_GUI_VISUALIZATION_FILTER_EDITOR_H

#include <QDialog>
#include <QVBoxLayout>

#include "Filter.h"
#include "FilterFactory.h"

class FilterEditor : public QDialog { Q_OBJECT
private:
    QVBoxLayout *create_layout;
    QMap<QString, FilterFactory *> factory_map;
public:
    FilterEditor();
    virtual ~FilterEditor();
    
    Filter *create_filter();
private slots:
    void set_factory(QString name);
};

#endif
