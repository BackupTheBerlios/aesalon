#ifndef AESALON_INTERFACE_GDB_PARSER_H
#define AESALON_INTERFACE_GDB_PARSER_H

#include <ostream>

#include "platform/BidirectionalPipe.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class Parser {
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
    Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe;
    
    void send_command(std::string command);
    
    bool is_gdb(std::string line);
public:
    Parser(Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe);
    virtual ~Parser();
    
    void parse(std::ostream &non_gdb);
    
    gdb_status_e get_status() const { return gdb_status; }
    void set_status(gdb_status_e new_status) { gdb_status = new_status; }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
