#include "EntryManager.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

Misc::SmartPointer<Entry> EntryManager::get_entry(EntryID id) const {
    entry_list_t::const_iterator i = entry_list.begin();
    for(; i != entry_list.end(); i ++) {
        if((*i)->get_id() == id) return *i;
    }
    return NULL;
}

Misc::SmartPointer<Entry> EntryManager::get_entry(std::string name) const {
    entry_list_t::const_iterator i = entry_list.begin();
    for(; i != entry_list.end(); i ++) {
        if((*i)->get_name() == name) return *i;
    }
    return NULL;
}

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon
