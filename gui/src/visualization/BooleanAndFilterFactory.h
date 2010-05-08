#ifndef AESALON_GUI_VISUALIZATION_BOOLEAN_AND_FILTER_FACTORY_H
#define AESALON_GUI_VISUALIZATION_BOOLEAN_AND_FILTER_FACTORY_H

#include "FilterFactory.h"

class BooleanAndFilterFactory : public FilterFactory {
public:
    BooleanAndFilterFactory();
    virtual ~BooleanAndFilterFactory();
    
    virtual Filter *create_filter();
    virtual QString get_name() const;
private:
    
};

#endif
