#include <string>
#include "Parser.h"
#include "misc/ArgumentParser.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

Parser::Parser(Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe)
    : gdb_status(GDB_STOPPED), bi_pipe(bi_pipe) {
    
    send_command("set can-use-hw-watchpoints 0");
    
    std::string command_string = "-exec-run ";
    for(std::size_t x = 1; x < Misc::ArgumentParser::get_instance()->get_files(); x ++)
        command_string += Misc::StreamAsString() << Misc::ArgumentParser::get_instance()->get_file(x)->get_filename() << " ";
    send_command(command_string);
    
    
}
Parser::~Parser() {
    
}

void Parser::parse(std::ostream &non_gdb) {
    std::string input = bi_pipe->get_string();
    
    if(!is_gdb(input)) non_gdb << input << std::endl;
    
    
    
    switch(get_status()) {
        case GDB_FINISHED: {
            send_command("quit");
            break;
        }
        default: break;
    }
}

bool Parser::is_gdb(std::string line) {
    return false;
}

void Parser::send_command(std::string command) {
    bi_pipe->send_string(command + "\n");
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
