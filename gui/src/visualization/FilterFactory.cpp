#include "FilterFactory.h"
#include "FilterFactory.moc"

FilterFactory::FilterFactory() {
    
}

FilterFactory::~FilterFactory() {
    
}

Filter *FilterFactory::create_filter() {
    return NULL;
}

QString FilterFactory::get_name() const {
    return "NULL";
}
