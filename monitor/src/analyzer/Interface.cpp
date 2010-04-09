#include "Interface.h"

namespace Analyzer {

Interface::Interface() {
    storage_manager = new StorageManager();
    first_file = NULL;
}

Interface::~Interface() {
    delete storage_manager;
}

File *Interface::parse_file(std::string filename) {
    if(file_map[filename]) return file_map[filename];
    
    File *file = new File(filename, storage_manager);
    file_map[filename] = file;
    if(first_file == NULL) first_file = file;
    return file;
}

File *Interface::get_file(std::string filename) const {
    return file_map.at(filename);
}

File *Interface::get_file() const {
    return first_file;
}


} // namespace Analyzer
