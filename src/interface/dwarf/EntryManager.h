#ifndef AESALON_INTERFACE_DWARF_ENTRY_MANAGER_H
#define AESALON_INTERFACE_DWARF_ENTRY_MANAGER_H

#include <string>
#include <vector>

#include "Entry.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
namespace DWARF {

class EntryManager {
public:
    typedef std::vector<Misc::SmartPointer<Entry> > entry_list_t;
    Misc::SmartPointer<Entry> head;
private:
    entry_list_t entry_list;
public:
    EntryManager();
    virtual ~EntryManager() {}
    
    Misc::SmartPointer<Entry> get_entry(EntryID id) const;
    Misc::SmartPointer<Entry> get_entry(std::string name) const;
    Misc::SmartPointer<Entry> get_head() const { return head; }
};

} // namespace DWARF
} // namespace Interface
} // namespace Aesalon

#endif
