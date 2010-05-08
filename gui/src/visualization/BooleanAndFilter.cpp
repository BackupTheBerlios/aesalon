#include "BooleanAndFilter.h"

bool BooleanAndFilter::passes_filter(Event *event) {
    Filter *filter = get_child();
    while(filter != NULL) {
        if(!filter->passes_filter(event)) return false;
        filter = filter->get_next();
    }
    return true;
}
