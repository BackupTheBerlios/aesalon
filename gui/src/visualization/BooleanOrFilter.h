#ifndef AESALON_GUI_VISUALIZATION_BOOLEAN_OR_FILTER_H
#define AESALON_GUI_VISUALIZATION_BOOLEAN_OR_FILTER_H

#include "Filter.h"

class BooleanOrFilter : public Filter {
public:
    virtual bool passes_filter(Event *event);
};

#endif
