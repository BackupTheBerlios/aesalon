#ifndef AESALON_INTERFACE_GDBPARSER_H
#define AESALON_INTERFACE_GDBPARSER_H

#include "platform/BidirectionalPipe.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {

class GDBParser {
public:
    enum gdb_status_e {
        GDB_STOPPED,
        GDB_RUNNING,
        GDB_PAUSED,
        GDB_FINISHED,
        GDB_STATUS_TYPES
    };
private:
    gdb_status_e gdb_status;
public:
    GDBParser();
    virtual ~GDBParser();
    
    bool parse_line(std::string line);
    
    gdb_status_e get_status() const { return gdb_status; }
};

}  // namespace Interface
}  // namespace Aesalon

#endif
