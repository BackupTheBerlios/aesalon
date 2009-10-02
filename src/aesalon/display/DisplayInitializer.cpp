#include <string>

#include "DisplayInitializer.h"
#include "misc/ArgumentParser.h"
#include "BasicDisplay.h"
#include "misc/Exception.h"

#ifdef AESALON_BUILD_STDOUT_INTERFACE
    #include "stdout/StdoutDisplay.h"
#endif


namespace Aesalon {
namespace Display {

template<> DisplayInitializer *Misc::Singleton<DisplayInitializer>::instance = 0;

DisplayInitializer::DisplayInitializer() : Misc::Singleton<DisplayInitializer>() {
    Misc::StringArgument *sa = Misc::ArgumentParser::get_instance()->
        get_argument("display interface").to<Misc::StringArgument>();
    
    std::string display_interface = sa->get_value();
    if(display_interface == "") {
        throw Misc::Exception("Requested initialization of an unspecified display interface");
    }
#ifdef AESALON_BUILD_STDOUT_INTERFACE
    else if(display_interface == "stdout") new Stdout::StdoutDisplay();
#endif
    else throw Misc::Exception("Unknown display interface requested: " + display_interface);
    
    BasicDisplay::get_instance()->start();
}

DisplayInitializer::~DisplayInitializer() {
    if(BasicDisplay::get_instance()) delete BasicDisplay::get_instance();
}

} // namespace Display
} // namespace Aesalon
