#include "CompilationUnit.h"
#include "Parser.h"

namespace Aesalon {
namespace Monitor {
namespace DWARF {

CompilationUnit::CompilationUnit(Misc::SmartPointer<ELF::Parser> elf_parser) {
    Misc::SmartPointer<Block> debug_info = elf_parser->get_section(".debug_info")->get_content();
    initial_offset = Parser::parse_u32(debug_info);
}

CompilationUnit::~CompilationUnit() {
    
}

} // namespace DWARF
} // namespace Monitor
} // namespace Aesalon
