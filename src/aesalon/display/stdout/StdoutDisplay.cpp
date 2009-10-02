#include "StdoutDisplay.h"
#include "interface/Program.h"
#include "misc/ArgumentParser.h"
#include "misc/Exception.h"

namespace Aesalon {
namespace Display {
namespace Stdout {

StdoutDisplay::StdoutDisplay() : BasicDisplay() {
    
}

StdoutDisplay::~StdoutDisplay() {
    
}

void StdoutDisplay::start() {
    Misc::ArgumentParser *ap = Misc::ArgumentParser::get_instance();
    
    if(!ap->get_files()) {
        throw Misc::Exception("No filenames in argument parser");
    }
    Interface::Program program;
    program.set_library_location(ap->get_argument("aesalon library path").to<Misc::StringArgument>()->get_value());
    program.set_filename(ap->get_file(0)->get_filename());
    if(ap->get_files() > 1) {
        std::string program_arguments = "";
        for(std::size_t x = 1; x < ap->get_files(); x ++) {
            program_arguments += ap->get_file(x)->get_filename();
        }
        program.set_arguments(program_arguments);
    }
    else program.set_arguments("");
    
    program.execute();
    
    sleep(1);
}

} // namespace Stdout
} // namespace Display
} // namespace Aesalon
