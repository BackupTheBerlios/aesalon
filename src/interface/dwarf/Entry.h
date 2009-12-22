#ifndef AESALON_INTERFACE_DWARF_ENTRY_H
#define AESALON_INTERFACE_DWARF_ENTRY_H

#include <string>

#include "EntryID.h"
#include "Tag.h"

namespace Aesalon {
namespace Interface {
namespace DWARF {

class Entry {
private:
    EntryID id;
    std::string name;
    Misc::SmartPointer<Tag> tag;
    
    Misc::SmartPointer<Entry> child;
public:
    Entry(std::string name = "");
    virtual ~Entry() {}
    
    EntryID get_id() const { return id; }
    
    std::string get_name() const { return name; }
    void set_name(std::string new_name) { name = new_name; }
    
    Misc::SmartPointer<Tag> get_tag() const { return tag; }
    void set_tag(Misc::SmartPointer<Tag> new_tag) { tag = new_tag; }
    
    Misc::SmartPointer<Entry> get_child() const { return child; }
    void set_child(Misc::SmartPointer<Entry> new_child) { child = new_child; }
};

} // namespace DWARF
} // namespace Interface
} // namespace Aesalon

#endif
