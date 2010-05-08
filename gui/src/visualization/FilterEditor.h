#ifndef AESALON_GUI_VISUALIZATION_FILTER_EDITOR_H
#define AESALON_GUI_VISUALIZATION_FILTER_EDITOR_H

#include <QDialog>
#include <QFormLayout>

#include "Filter.h"

class FilterEditor : public QDialog { Q_OBJECT
private:
    QFormLayout *create_layout;
public:
    FilterEditor();
    virtual ~FilterEditor();
    
    Filter *create_filter();
};

#endif
