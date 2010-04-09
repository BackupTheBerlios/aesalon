#include "Interface.h"

namespace Analyzer {

Interface::Interface() {
    storage_manager = new StorageManager();
}

Interface::~Interface() {
    delete storage_manager;
}

File *Interface::parse_file(std::string filename) {
    File *file = new File(filename);
    return file;
}

File *Interface::get_file(std::string filename) const {
    return file_map.at(filename);
}


} // namespace Analyzer
