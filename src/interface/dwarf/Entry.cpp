#include "Entry.h"

namespace Aesalon {
namespace Interface {
namespace DWARF {

Entry::Entry(std::string name) : name(name) {
    /* NOTE: this is not thread-safe */
    static EntryID last_id = 0;
    id = ++last_id;
}

} // namespace DWARF
} // namespace Interface
} // namespace Aesalon
