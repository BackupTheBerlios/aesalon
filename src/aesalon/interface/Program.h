#ifndef AESALON_INTERFACE_PROGRAM_H
#define AESALON_INTERFACE_PROGRAM_H

#include <string>

#include "Pipe.h"
#include "misc/SmartPointer.h"
#include "ProgramSymbolParser.h"

namespace Aesalon {
namespace Interface {
/* NOTE: forward declaration. */
class PipeListener;

class Program {
private:
    std::string filename;
    std::string arguments;
    std::string library_location;
    Misc::SmartPointer<Pipe> program_pipe;
    Misc::SmartPointer<PipeListener> pipe_listener;
    Misc::SmartPointer<ProgramSymbolParser> program_parser;
public:
    Program() {}
    virtual ~Program();
    
    void set_filename(std::string new_filename) { filename = new_filename; }
    std::string get_filename() const { return filename; }
    void set_arguments(std::string new_arguments) { arguments = new_arguments; }
    std::string get_arguments() const { return arguments; }
    void set_library_location(std::string new_location) { library_location = new_location; }
    std::string get_library_location() const { return library_location; }
    
    void execute();
    
    std::string resolve_address(std::size_t address);
    
    bool is_running();
private:
    void create_listening_thread();
};

} // namespace Interface
} // namespace Aesalon

#endif
