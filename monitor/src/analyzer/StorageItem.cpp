#include "StorageItem.h"
#include <cstring>

namespace Analyzer {

StorageItem::StorageItem() {

}

StorageItem::~StorageItem() {

}

void StorageItem::add_attribute(StorageAttribute *attribute) {
    attribute_list.push_back(attribute);
}

StorageAttribute *StorageItem::get_attribute(const std::string &name) {
    for(attribute_list_t::iterator i = attribute_list.begin(); i != attribute_list.end(); i ++) {
        if(!strcmp(name.c_str(), (*i)->get_name())) return (*i);
    }
    return NULL;
}

} // namespace Analyzer
