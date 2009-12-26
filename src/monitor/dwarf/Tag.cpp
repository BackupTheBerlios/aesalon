#include <iostream>

#include "Tag.h"
#include "Parser.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

Misc::SmartPointer<Tag> Tag::parse_from(Misc::SmartPointer<Block> block) {
    Word tag_type = Parser::parse_uleb128(block);
    
    Misc::SmartPointer<Tag> tag;
    
    /* If the tag type is 0, then the tag is a NULL tag (section 7.5.2, paragraph 2). */
    if(tag_type == 0x00) return new Tag("NULL parsed tag");
    
    std::string tag_type_string = "";
    std::map<Word, std::string> tag_type_map;
    /* NOTE: these are from the DWARF 3 documentation (Figure 18). */
    /* TODO: complete this list. */
    /* TODO: find a better way to encapsulate these values. */
    tag_type_map[0x01] = "DW_TAG_array_type";
    tag_type_map[0x02] = "DW_TAG_class_type";
    tag_type_map[0x03] = "DW_TAG_entry_type";
    tag_type_map[0x04] = "DW_TAG_enumeration_type";
    tag_type_map[0x05] = "DW_TAG_formal_parameter";
    tag_type_map[0x08] = "DW_TAG_imported_declaration";
    tag_type_map[0x0a] = "DW_TAG_label";
    tag_type_map[0x0b] = "DW_TAG_lexical_block";
    tag_type_map[0x0d] = "DW_TAG_member";
    tag_type_map[0x0f] = "DW_TAG_pointer_type";
    tag_type_map[0x10] = "DW_TAG_reference_type";
    tag_type_map[0x11] = "DW_TAG_compile_unit";
    tag_type_map[0x12] = "DW_TAG_string_type";
    tag_type_map[0x13] = "DW_TAG_structure_type";
    tag_type_map[0x15] = "DW_TAG_subroutine_type";
    tag_type_map[0x16] = "DW_TAG_typedef";
    
    tag_type_string = tag_type_map[tag_type];
    
    tag = new Tag(tag_type_string);
    
    std::cout << "New Tag parsed, type is \"" << tag_type_string << "\"\n";
    
    return tag;
}

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon
