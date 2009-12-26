#ifndef AESALON_MONITOR_DWARF_TAG_H
#define AESALON_MONITOR_DWARF_TAG_H

#include "Types.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

class Tag {
private:
    std::string type;
public:
    Tag(std::string type) : type(type) {}
    virtual ~Tag() {}
    
    static Misc::SmartPointer<Tag> parse_from(Misc::SmartPointer<Block> block);
};

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon

#endif
