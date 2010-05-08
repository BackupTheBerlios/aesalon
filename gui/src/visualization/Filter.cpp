#include "Filter.h"

Filter::Filter() {

}

Filter::~Filter() {

}

void Filter::add_child(Filter *new_child) {
    if(child == NULL) {
        child = new_child;
        child->next = NULL;
        return;
    }
    
    while(child->get_next()) child = child->get_next();
    
    child->set_next(new_child);
    new_child->set_next(NULL);
}

bool Filter::passes_filter(Event *event) {
    return false;
}
