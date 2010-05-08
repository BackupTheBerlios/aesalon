#include "BooleanOrFilter.h"

bool BooleanOrFilter::passes_filter(Event *event) {
    Filter *filter = get_child();
    while(filter != NULL) {
        if(filter->passes_filter(event)) return true;
    }
    return false;
}
