#include "Parser.h"

namespace Aesalon {
namespace Interface {
namespace DWARF {

Word Parser::parse_leb128(Block block, bool is_signed) {
    Word result = 0;
    int shift = 0;
    Word offset = 0;
    
    while(true) {
        if(is_signed) {
            break;
        }
        else {
            Byte next = block[offset++];
            result |= (next & 0x7f) << shift;
            if(next & 0x80) break;
        }
        shift += 7;
    }
    
    block.erase(block.begin(), block.begin() + offset);
    
    return result;
}

} // namespace DWARF
} // namespace Interface
} // namespace Aesalon
