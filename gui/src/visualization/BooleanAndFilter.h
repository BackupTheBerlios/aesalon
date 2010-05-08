#ifndef AESALON_GUI_VISUALIZATION_BOOLEAN_AND_FILTER_H
#define AESALON_GUI_VISUALIZATION_BOOLEAN_AND_FILTER_H

#include "Filter.h"

class BooleanAndFilter : public Filter {
public:
    virtual bool passes_filter(Event *event);
};

#endif
