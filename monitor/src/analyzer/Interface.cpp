#include "Interface.h"

namespace Analyzer {

Interface::Interface() {

}

Interface::~Interface() {

}

File *Interface::parse_file(std::string filename) {
    File *file = new File(filename);
    return file;
}

File *Interface::get_file(std::string filename) const {
    return file_map.at(filename);
}


} // namespace Analyzer
