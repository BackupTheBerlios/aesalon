#ifndef AESALON_MONITOR_DWARF_ABBREV_PARSER_H
#define AESALON_MONITOR_DWARF_ABBREV_PARSER_H

#include <vector>

#include "Types.h"
#include "elf/Parser.h"
#include "Tag.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

class AbbrevParser {
protected:
    typedef std::vector<Misc::SmartPointer<Tag> > tag_list_t;
private:
    tag_list_t tag_list;
public:
    AbbrevParser(Misc::SmartPointer<ELF::Parser> elf_parser);
    virtual ~AbbrevParser();
    
    Misc::SmartPointer<Tag> get_tag(std::size_t which) const { return tag_list[which]; }
};

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon

#endif
