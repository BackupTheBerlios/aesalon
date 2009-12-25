#include "Entry.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

Entry::Entry(std::string name) : name(name) {
    /* NOTE: this is not thread-safe */
    static EntryID last_id = 0;
    id = ++last_id;
}

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon
