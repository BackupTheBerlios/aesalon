#ifndef AESALON_GUI_VISUALIZATION_FILTER_FACTORY_H
#define AESALON_GUI_VISUALIZATION_FILTER_FACTORY_H

#include <QWidget>
#include "Filter.h"

class FilterFactory : public QWidget { Q_OBJECT
public:
    FilterFactory();
    virtual ~FilterFactory();
    
    virtual Filter *create_filter();
    virtual QString get_name() const;
};

#endif
