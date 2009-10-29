#include "GDBParser.h"
#include <string>

namespace Aesalon {
namespace Interface {

GDBParser::GDBParser() : gdb_status(GDB_STOPPED) {
    
}
GDBParser::~GDBParser() {
    
}

bool GDBParser::parse_line(std::string line) {
    return false;
}

}  // namespace Interface
}  // namespace Aesalon
